#include <common.h>
#include <gpu.h>
#include <logger.h>
#include <opencv2/core/hal/interface.h>
#include <cstdlib>
#include <cstring>

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
