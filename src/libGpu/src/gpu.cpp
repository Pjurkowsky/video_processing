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
  uint8_t* frames = (uint8_t*)malloc(frame_mem_size * frame_count);
  auto capture = video.getVideoCapture();
  capture.set(cv::CAP_PROP_POS_FRAMES, 0);

  for (int i = 0; i < frame_count; i++) {
    capture >> frame;
    memcpy(frames + frame_mem_size * i, frame.data, frame_mem_size);
  }
  utill::benchmark("GPU:", [&frames, &size, &frame_count]() {
    gpu::bgr_to_mono(frames, frame_count, size.height, size.width);
  });
  free(frames);
}
