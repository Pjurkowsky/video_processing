#include <gpu.h>
#include <opencv2/core/hal/interface.h>
#include <cstring>
#include "common.h"

void gpu::gpu(Video video) {
  cv::Mat frame = video.getFrame(0);
  cv::Size size = frame.size();
  benchmark([&frame, &size]() {
    gpu::bgr_to_mono(frame.data, size.height, size.width);
  });
  video.displayFrame(frame);
}
