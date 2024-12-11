#pragma once

#include <argparse.hpp>
#include <common.h>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <opencv2/opencv.hpp>
#include <opencv2/videoio.hpp>
#include <queue>
#include <thread>
#include <vector>
#include <video.h>

void processResizeFrames(std::queue<cv::Mat> &frameQueue,
                         cv::VideoWriter &writer, double scaleFactor);
void processMonoFrames(std::queue<cv::Mat> &frameQueue,
                       cv::VideoWriter &writer);

namespace cpu {
void cpu(Video video, utill::filter filter, int batchSize, std::string out,
         int width, int height);
cv::Ptr<cv::VideoWriter> createVideoWriter(std::string filenem);
cv::Ptr<cv::VideoCapture> createVideoReader(std::string filenem);
void apply_transform(utill::filter operation);

} // namespace cpu
