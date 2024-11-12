#include "cpu.h"

std::mutex queueMutex;
std::condition_variable queueCV;
std::condition_variable writeCV;
bool finishedReading = false;

void processResizeFrames(std::queue<std::pair<int, cv::Mat>>& frameQueue, std::map<int, cv::Mat>& processedFrames,
                         double scaleFactor) {
  while (true) {
    std::pair<int, cv::Mat> framePair;
    {
      std::unique_lock<std::mutex> lock(queueMutex);
      queueCV.wait(lock, [&]() {
        return !frameQueue.empty() || finishedReading;
      });

      if (frameQueue.empty() && finishedReading)
        break;

      framePair = frameQueue.front();
      frameQueue.pop();
    }

    int frameIndex = framePair.first;
    cv::Mat resizedFrame;
    cv::resize(framePair.second, resizedFrame,
               cv::Size(framePair.second.cols * scaleFactor, framePair.second.rows * scaleFactor), 0, 0,
               cv::INTER_LINEAR);

    {
      std::lock_guard<std::mutex> lock(queueMutex);
      processedFrames[frameIndex] = resizedFrame;
    }
    writeCV.notify_one();
  }
}

void processMonoFrames(std::queue<std::pair<int, cv::Mat>>& frameQueue, std::map<int, cv::Mat>& processedFrames) {
  while (true) {
    std::pair<int, cv::Mat> framePair;
    {
      std::unique_lock<std::mutex> lock(queueMutex);
      queueCV.wait(lock, [&]() {
        return !frameQueue.empty() || finishedReading;
      });

      if (frameQueue.empty() && finishedReading)
        break;

      framePair = frameQueue.front();
      frameQueue.pop();
    }

    int frameIndex = framePair.first;
    cv::Mat monoFrame;
    cv::cvtColor(framePair.second, monoFrame, cv::COLOR_BGR2GRAY);

    {
      std::lock_guard<std::mutex> lock(queueMutex);
      processedFrames[frameIndex] = monoFrame;
    }
    writeCV.notify_one();
  }
}

void cpu(Video video, std::string filter) {
  int totalFrames = video.getVideoCapture().get(cv::CAP_PROP_FRAME_COUNT);
  double fps = video.getVideoCapture().get(cv::CAP_PROP_FPS);

  cv::VideoWriter outputVideo;
  cv::VideoCapture capture = video.getVideoCapture();
  cv::Mat frame;
  capture >> frame;
  if (frame.empty()) {
    std::cerr << "Error: Could not read the first frame!" << std::endl;
    return;
  }

  double scaleFactor = 0.5;

  if (filter == "mono") {
    cv::cvtColor(frame, frame, cv::COLOR_BGR2GRAY);
  } else if (filter == "resize") {
    cv::resize(frame, frame, cv::Size(frame.cols * scaleFactor, frame.rows * scaleFactor), 0, 0, cv::INTER_LINEAR);
  }

  bool isMono = frame.channels() == 1;
  outputVideo.open("output_video.mp4", cv::VideoWriter::fourcc('a', 'v', 'c', '1'), fps, frame.size(), !isMono);
  if (!outputVideo.isOpened()) {
    std::cerr << "Could not open the output video for write: output_video.mp4" << std::endl;
    return;
  }

  std::queue<std::pair<int, cv::Mat>> frameQueue;
  std::map<int, cv::Mat> processedFrames;
  int nextFrameToWrite = 0;
  int numThreads = std::thread::hardware_concurrency();
  std::vector<std::thread> threads;

  if (filter == "mono") {
    for (int i = 0; i < numThreads; ++i) {
      threads.emplace_back([&]() {
        processMonoFrames(frameQueue, processedFrames);
      });
    }
  } else if (filter == "resize") {
    for (int i = 0; i < numThreads; ++i) {
      threads.emplace_back([&]() {
        processResizeFrames(frameQueue, processedFrames, scaleFactor);
      });
    }
  }

  utill::benchmark("Frame processing time", [&]() {
    for (int i = 0; i < totalFrames; ++i) {
      capture >> frame;
      if (frame.empty())
        break;

      {
        std::lock_guard<std::mutex> lock(queueMutex);
        frameQueue.push({i, frame});
      }
      queueCV.notify_one();
    }

    {
      std::lock_guard<std::mutex> lock(queueMutex);
      finishedReading = true;
    }
    queueCV.notify_all();

    std::thread writerThread([&]() {
      while (nextFrameToWrite < totalFrames - 1) {
        std::unique_lock<std::mutex> lock(queueMutex);
        writeCV.wait(lock, [&]() {
          return processedFrames.count(nextFrameToWrite) > 0 || finishedReading;
        });

        while (processedFrames.count(nextFrameToWrite) > 0) {
          outputVideo.write(processedFrames[nextFrameToWrite]);
          processedFrames.erase(nextFrameToWrite);
          ++nextFrameToWrite;
        }
      }
    });
    writerThread.join();
  });

  for (auto& t : threads) {
    t.join();
  }

  outputVideo.release();
}
