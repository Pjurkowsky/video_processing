#include <common.h>
#include <gpu.h>
#include <logger.h>
#include <opencv2/core/hal/interface.h>
#include <chrono>
#include <cstdlib>
#include <cstring>

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

void gpu::gpu(Video video, std::string operation, int batch_size) {
  int dst_width = 640;
  int dst_height = 480;

  double fps = video.getVideoCapture().get(cv::CAP_PROP_FPS);
  int frame_count = static_cast<int>(video.getVideoCapture().get(cv::CAP_PROP_FRAME_COUNT));
  cv::Mat frame = video.getFrame(0);
  cv::Size size = frame.size();
  int frame_mem_size = size.height * size.width * 3 * sizeof(uint8_t);
  int total_time = 0;
  total_time = utill::silent_benchmark([]() {});

  auto capture = video.getVideoCapture();
  capture.set(cv::CAP_PROP_POS_FRAMES, 0);

  if (operation == "resize") {
    uint8_t* src_buffer;
    uint8_t* dst_buffer;
    gpu::malloc_memory(&src_buffer, (sizeof(uint8_t) * size.height * size.width * 3 * batch_size));
    gpu::malloc_memory(&dst_buffer, (sizeof(uint8_t) * dst_width * dst_height * 3 * batch_size));

    uint8_t* batch_buffer = new uint8_t[batch_size * frame_mem_size];
    uint8_t* resized_batch_buffer = new uint8_t[batch_size * dst_height * dst_width * 3 * sizeof(uint8_t)];

    cv::VideoWriter videoWriter(
        "output_video.mp4", cv::VideoWriter::fourcc('X', '2', '6', '4'), fps, cv::Size(dst_width, dst_height));

    for (int i = 0; i < frame_count; i += batch_size) {
      INFO_LOG << "Frames " << i << "/" << frame_count;
      int current_batch_size = std::min(batch_size, frame_count - i);

      for (int j = 0; j < current_batch_size; ++j) {
        capture >> frame;
        std::memcpy(batch_buffer + j * frame_mem_size, frame.data, frame_mem_size);
      }
      total_time += utill::silent_benchmark([&]() {
        gpu::resize(batch_buffer,
                    current_batch_size,
                    size.height,
                    size.width,
                    dst_height,
                    dst_width,
                    resized_batch_buffer,
                    src_buffer,
                    dst_buffer);
      });

      for (int j = 0; j < current_batch_size; ++j) {
        cv::Mat resized_frame(dst_height, dst_width, CV_8UC3, resized_batch_buffer + j * dst_height * dst_width * 3);
        videoWriter.write(resized_frame);
      }
    }
    delete[] batch_buffer;
    delete[] resized_batch_buffer;
    gpu::free_memory(src_buffer);
    gpu::free_memory(dst_buffer);
    videoWriter.release();
  } else if (operation == "mono") {
    uint8_t* buffer;
    gpu::malloc_memory(&buffer, (sizeof(uint8_t) * size.height * size.width * 3 * batch_size));
    uint8_t* batch_buffer = new uint8_t[batch_size * frame_mem_size];

    cv::VideoWriter videoWriter("output_video.mp4", cv::VideoWriter::fourcc('X', '2', '6', '4'), fps, size);
    for (int i = 0; i < frame_count; i += batch_size) {
      int current_batch_size = std::min(batch_size, frame_count - i);

      for (int j = 0; j < current_batch_size; ++j) {
        capture >> frame;
        std::memcpy(batch_buffer + j * frame_mem_size, frame.data, frame_mem_size);
      }

      utill::benchmark("GPU:", [&]() {
        gpu::bgr_to_mono(batch_buffer, current_batch_size, size.height, size.width, buffer);
      });

      for (int j = 0; j < current_batch_size; ++j) {
        cv::Mat processed_frame(size.height, size.width, CV_8UC3, batch_buffer + j * frame_mem_size);
        videoWriter.write(processed_frame);
      }
    }
    delete[] batch_buffer;
    gpu::free_memory(buffer);
    videoWriter.release();
  }
  LOG(LEVEL_LOG) << "Applying a transform took " << total_time << "ms";
}
