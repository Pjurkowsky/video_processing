#include "argparse.hpp"
#include <chrono>
#include <common.h>
#include <cstdlib>
#include <cstring>
#include <gpu.h>
#include <logger.h>
#include <opencv2/core/cuda.hpp>
#include <opencv2/core/hal/interface.h>
#include <opencv2/core/mat.hpp>
#include <opencv2/cudacodec.hpp>

#define LOG(level) TaggedLogStream("GPU", level)

#define ERROR_LOG LOG(LEVEL_ERROR)
#define INFO_LOG LOG(LEVEL_INFO)
#define WARNING_LOG LOG(LEVEL_WARNING)

void gpu::gpu(Video video) {
  const int frame_count = 60;
  cv::Mat frame = video.getFrame(0);
  cv::Size size = frame.size();
  int frame_mem_size = size.height * size.width * 3 * sizeof(uint8_t);
  auto capture = video.getVideoCapture();
  capture.set(cv::CAP_PROP_POS_FRAMES, 0);
  capture >> frame;
  cv::Mat mono_frame = frame.clone();
  utill::benchmark("GPU:", [&mono_frame, &size, &frame_count]() {
    gpu::bgr_to_mono(mono_frame.data, size.height, size.width);
  });
  cv::Mat concatenatedFrame;
  cv::hconcat(frame, mono_frame, concatenatedFrame);
  video.displayFrame(concatenatedFrame);
}

void gpu::gpu(Video video, utill::filter operation, int batch_size,
              std::string out, int width, int height) {
  int dst_width = width;
  int dst_height = height;

  double fps = video.getVideoCapture().get(cv::CAP_PROP_FPS);
  int frame_count =
      static_cast<int>(video.getVideoCapture().get(cv::CAP_PROP_FRAME_COUNT));
  cv::Mat frame = video.getFrame(0);
  cv::Size size = frame.size();
  int frame_mem_size = size.height * size.width * 3 * sizeof(uint8_t);

  int total_time = 0;
  int write_time = 0;
  int read_time = 0;

  auto capture = video.getVideoCapture();
  capture.set(cv::CAP_PROP_POS_FRAMES, 0);

  if (operation == utill::RESIZE) {
    uint8_t *src_buffer;
    uint8_t *dst_buffer;
    gpu::malloc_memory(&src_buffer, (sizeof(uint8_t) * size.height *
                                     size.width * 3 * batch_size));

    gpu::malloc_memory(&dst_buffer, (sizeof(uint8_t) * dst_width * dst_height *
                                     3 * batch_size));

    cv::cuda::GpuMat src_mat(size.height * batch_size, size.width * 3, CV_8UC1,
                             src_buffer);
    cv::cuda::GpuMat dst_mat(size.height * batch_size, size.width * 3, CV_8UC1,
                             dst_buffer);

    uint8_t *batch_buffer = new uint8_t[batch_size * frame_mem_size];
    uint8_t *resized_batch_buffer =
        new uint8_t[batch_size * dst_height * dst_width * 3 * sizeof(uint8_t)];

    // cv::VideoWriter videoWriter(out,
    //                             cv::VideoWriter::fourcc('X', '2', '6', '4'),
    //                             fps, cv::Size(dst_width, dst_height));

    cv::Ptr<cv::cudacodec::VideoWriter> cudaVideoWriter;
    cv::Ptr<cv::cudacodec::VideoReader> cudaVideoReader;

    cudaVideoReader = cv::cudacodec::createVideoReader(video.filePath);

    for (int i = 0; i < frame_count; i += batch_size) {
      INFO_LOG << "Frames " << i << "/" << frame_count;
      int current_batch_size = std::min(batch_size, frame_count - i);

      read_time += utill::silent_benchmark([&]() {
        for (int j = 0; j < current_batch_size; ++j) {
          cv::cuda::GpuMat gpu_frame;
          cudaVideoReader->nextFrame(gpu_frame);
          gpu::copyDeviceDevice(gpu_frame.data, src_buffer + j * frame_mem_size,
                                frame_mem_size);
          // HERE
          // capture >> frame;
          // std::memcpy(batch_buffer + j * frame_mem_size, frame.data,
          //             frame_mem_size);
        }
      });
      total_time += utill::silent_benchmark([&]() {
        gpu::resize(batch_buffer, current_batch_size, size.height, size.width,
                    dst_height, dst_width, resized_batch_buffer, src_buffer,
                    dst_buffer);
      });
      write_time += utill::silent_benchmark([&]() {
        if (!cudaVideoWriter) {
          cudaVideoWriter = cv::cudacodec::createVideoWriter(
              out, cv::Size(dst_width, dst_height),

              cv::cudacodec::H264, fps);
        }
        if (!cudaVideoWriter) {
          ERROR_LOG << "Could not create CUDA Video Writer";
        }
        for (int j = 0; j < current_batch_size; ++j) {
          cv::cuda::GpuMat resized_frame(dst_height, dst_width, CV_8UC3,
                                         dst_buffer +
                                             j * dst_height * dst_width * 3);
          cudaVideoWriter->write(resized_frame);
        }
      });
    }
    delete[] batch_buffer;
    delete[] resized_batch_buffer;
    gpu::free_memory(src_buffer);
    gpu::free_memory(dst_buffer);
    // videoWriter.release();
  } else if (operation == utill::MONO) {
    uint8_t *buffer;
    gpu::malloc_memory(&buffer, (frame_mem_size * batch_size));
    // int frame_mem_size = size.height * size.width * 3 * sizeof(uint8_t);
    uint8_t *batch_buffer;
    gpu::malloc_memory(&batch_buffer, (frame_mem_size * batch_size));

    cv::Ptr<cv::cudacodec::VideoWriter> cudaVideoWriter;
    cv::Ptr<cv::cudacodec::VideoReader> cudaVideoReader;
    cudaVideoReader = cv::cudacodec::createVideoReader(video.filePath);

    for (int i = 0; i < frame_count; i += batch_size) {
      INFO_LOG << "Frames " << i << "/" << frame_count;
      int current_batch_size = std::min(batch_size, frame_count - i);

      read_time += utill::silent_benchmark([&]() {
        for (int j = 0; j < current_batch_size; ++j) {
          cv::cuda::GpuMat gpu_frame;
          cudaVideoReader->nextFrame(gpu_frame);
          gpu::copyDeviceDevice(gpu_frame.data,
                                batch_buffer + j * frame_mem_size,
                                frame_mem_size);
        }
      });

      total_time += utill::silent_benchmark([&]() {
        gpu::bgr_to_mono(batch_buffer, current_batch_size, size.height,
                         size.width, buffer);
      });

      write_time += utill::silent_benchmark([&]() {
        for (int j = 0; j < current_batch_size; ++j) {
          if (!cudaVideoWriter) {
            cudaVideoWriter = cv::cudacodec::createVideoWriter(
                out, cv::Size(size.width, size.height), cv::cudacodec::H264,
                fps);
          }
          if (!cudaVideoWriter) {
            ERROR_LOG << "Could not create CUDA Video Writer";
          }

          cv::cuda::GpuMat processed_frame_gpu(size.height, size.width, CV_8UC3,
                                               buffer + j * frame_mem_size);
          cudaVideoWriter->write(processed_frame_gpu);
        }
      });
    }
    gpu::free_memory(batch_buffer);
    gpu::free_memory(buffer);
  }
  LOG(LEVEL_LOG) << "Decoding time " << read_time << "ms";
  LOG(LEVEL_LOG) << "Processing time " << total_time << "ms";
  LOG(LEVEL_LOG) << "Encoding time " << write_time << "ms";
}
