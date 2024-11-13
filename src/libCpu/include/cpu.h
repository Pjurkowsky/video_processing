#pragma once

#include <common.h>
#include <video.h>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <opencv2/opencv.hpp>
#include <queue>
#include <thread>
#include <vector>

void cpu(Video video, std::string filter, int batchSize);
void processResizeFrames(std::queue<cv::Mat>& frameQueue, cv::VideoWriter& writer, double scaleFactor);
void processMonoFrames(std::queue<cv::Mat>& frameQueue, cv::VideoWriter& writer);