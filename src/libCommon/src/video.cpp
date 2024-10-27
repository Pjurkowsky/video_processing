#include "video.h"
#include <logger.h>
#include <iostream>

#define LOG(level) LogTag("VIDEO", level)

Video::Video() : loaded(false) {}

Video::~Video() {
  if (videoCapture.isOpened()) {
    videoCapture.release();
  }
}

bool Video::loadFile(const std::string& filePath) {
  this->filePath = filePath;

  videoCapture.open(filePath);

  if (!videoCapture.isOpened()) {
    LOG(ERROR) << "Could not load video file " << filePath;
    loaded = false;
    return false;
  }

  LOG(INFO) << "Video file loaded successfully: " << filePath;
  loaded = true;
  return true;
}

cv::VideoCapture Video::getVideoCapture() {
  return videoCapture;
}

void Video::play() {
  if (!loaded || !videoCapture.isOpened()) {
    LOG(ERROR) << "Error: Video not loaded properly!";
    return;
  }

  double fps = videoCapture.get(cv::CAP_PROP_FPS);
  if (fps <= 0) {
    LOG(ERROR) << "Error: Could not retrieve frame rate.";
    return;
  }

  int frameDelay = static_cast<int>(1000.0 / fps);

  cv::Mat frame;
  while (true) {
    videoCapture >> frame;

    if (frame.empty()) {
      LOG(INFO) << "End of video.";
      break;
    }

    cv::imshow("Video Playback", frame);

    if (cv::waitKey(frameDelay) == 'q') {
      break;
    }
  }

  videoCapture.release();
  cv::destroyAllWindows();
}

cv::Mat Video::getFrame(int frameNumber) {
  cv::Mat frame;
  videoCapture.set(cv::CAP_PROP_POS_FRAMES, frameNumber);

  videoCapture >> frame;

  if (frame.empty()) {
    LOG(ERROR) << "Could not retrieve frame " << frameNumber;
  }

  return frame;
}

void Video::displayFrame(cv::Mat frame) {

  if (frame.empty()) {
    LOG(ERROR) << "Could not retrieve frame";
    return;
  }

  cv::imshow("", frame);

  if (cv::waitKey(0) == 'q') {
    return;
  }
}

bool Video::isLoaded() const {
  return loaded;
}
cv::Mat Video::applyBlur(const cv::Mat& frame) {
  cv::Mat blurredFrame;
  cv::blur(frame, blurredFrame, cv::Size(5, 5));
  return blurredFrame;
}
