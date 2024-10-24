#include <cpu.h>
#include <thread>

void cpu(Video video) {
  std::cout << "Hello from the CPU library" << std::endl;
  unsigned int n = std::thread::hardware_concurrency();
  std::cout << n << " concurrent threads are supported.\n";

  cv::Mat frame = video.getFrame(0);
  cv::Mat filteredFrame = video.applyBlur(frame);

  cv::Mat concatenatedFrame;
  cv::hconcat(frame, filteredFrame, concatenatedFrame);

  video.displayFrame(concatenatedFrame);
}
