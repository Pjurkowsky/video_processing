#include "logger.h"
#include <opencv-cuda.hpp>
#include <opencv2/core/cvstd.hpp>
#include <opencv2/core/cvstd_wrapper.hpp>
#include <opencv2/cudacodec.hpp>
#include <opencv2/highgui.hpp>

void gpu::opencv_cuda::test(char *filename) {

  cv::String str = filename;
  cv::Ptr<cv::cudacodec::VideoReader> video_reader =
      cv::cudacodec::createVideoReader(str);

  std::vector<cv::cuda::GpuMat> gpu_frames; // GpuMat to hold the GPU frame
  cv::Mat cpu_frame; // Mat to hold the frame after transferring to CPU

  while (true) {
    // Read the next frame from the video
    cv::cuda::GpuMat mat;
    bool success = video_reader->nextFrame(mat);
    if (!success)
      break; // Break if no more frames
    gpu_frames.push_back(mat);
    // Optionally process the frame here on GPU
    // Example: Convert to grayscale (if needed)
    // cv::cuda::GpuMat gray_frame;
    // cv::cuda::cvtColor(gpu_frame, gray_frame, cv::COLOR_BGR2GRAY);

    // Transfer the frame from GPU to CPU for display or further processing
    // gpu_frame.download(cpu_frame);

    // Display the frame
    // cv::imshow("CUDA Video Frame", cpu_frame);
    // if (cv::waitKey(30) >= 0)
    //   break;  // Exit on key press
  }
  TaggedLogStream("GPU_MAIN", LEVEL_INFO)
      << "Decoded " << gpu_frames.size() << " frames";
}
