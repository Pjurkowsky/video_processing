#pragma once

#include <opencv2/opencv.hpp>
#include <string>

class Video {
 public:
  Video();

  ~Video();

  bool loadFile(const std::string& filePath);

  cv::VideoCapture getVideoCapture();

  void play();

  cv::Mat getFrame(int frameNumber);

  void displayFrame(cv::Mat frame);

  cv::Mat applyBlur(const cv::Mat& frame);

  bool isLoaded() const;

 private:
  std::string filePath;
  bool loaded;
  cv::VideoCapture videoCapture;
};
