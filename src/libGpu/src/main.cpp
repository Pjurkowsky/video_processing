#include <gpu.h>
#include <chrono>
#include <iostream>
#include <opencv-cuda.hpp>
#include <thread>
#include "common.h"
#include "logger.h"
#include "video.h"

int main(int argc, char** argv) {
  std::cout << "GPU main" << std::endl;
  utill::benchmark("GPU Main", [&]() {
    gpu::opencv_cuda::test(argv[1]);
  });

  utill::benchmark("delay", []() {
    std::this_thread::sleep_for(std::chrono::seconds(1));
  });

  utill::benchmark("CPU", [&]() {
    Video vid;
    vid.loadFile(argv[1]);
    auto capture = vid.getVideoCapture();
    std::vector<cv::Mat> frames;
    while (capture.grab()) {
      cv::Mat frame;
      capture >> frame;
      frames.push_back(frame);
    }
    TaggedLogStream("CPU", LEVEL_INFO) << "Decoded " << frames.size() << " frames";
  });
  return 0;
}
