#include <cpu.h>

void cpu(Video video) {
  std::cout << "Hello from the CPU library" << std::endl;

  cv::Mat frame = video.getFrame(0);
  cv::Mat filteredFrame = video.applyBlur(frame);

  cv::Mat concatenatedFrame;
  cv::hconcat(frame, filteredFrame, concatenatedFrame);

  video.displayFrame(concatenatedFrame);
}
