#include "cpu.h"

std::mutex queueMutex;
std::condition_variable queueCV;
bool finishedReading = false;

void processResizeFrames(std::queue<cv::Mat>& frameQueue, cv::VideoWriter& writer, double scaleFactor) {
  while (true) {
    cv::Mat frame;
    {
      std::unique_lock<std::mutex> lock(queueMutex);
      queueCV.wait(lock, [&]() {
        return !frameQueue.empty() || finishedReading;
      });

      if (frameQueue.empty() && finishedReading)
        break;
      frame = frameQueue.front();
      frameQueue.pop();
    }

    cv::Mat resizedFrame;
    cv::resize(frame, resizedFrame, cv::Size(frame.cols * scaleFactor, frame.rows * scaleFactor), 0, 0,
               cv::INTER_LINEAR);

    {
      std::lock_guard<std::mutex> writerLock(queueMutex);
      writer.write(resizedFrame);
    }
  }
}

void processMonoFrames(std::queue<cv::Mat>& frameQueue, cv::VideoWriter& writer) {
  while (true) {
    cv::Mat frame;
    {
      std::unique_lock<std::mutex> lock(queueMutex);
      queueCV.wait(lock, [&]() {
        return !frameQueue.empty() || finishedReading;
      });

      if (frameQueue.empty() && finishedReading)
        break;
      frame = frameQueue.front();
      frameQueue.pop();
    }

    cv::Mat monoFrame;
    cv::cvtColor(frame, monoFrame, cv::COLOR_BGR2GRAY);

    {
      std::lock_guard<std::mutex> writerLock(queueMutex);
      writer.write(monoFrame);
    }
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

  std::queue<cv::Mat> frameQueue;
  int numThreads = std::thread::hardware_concurrency();
  std::vector<std::thread> threads;

  if (filter == "mono") {
    for (int i = 0; i < numThreads; ++i) {
      threads.emplace_back(processMonoFrames, std::ref(frameQueue), std::ref(outputVideo));
    }
  } else if (filter == "resize") {
    for (int i = 0; i < numThreads; ++i) {
      threads.emplace_back(processResizeFrames, std::ref(frameQueue), std::ref(outputVideo), scaleFactor);
    }
  }

  utill::benchmark("Frame processing time", [&]() {
    for (int i = 0; i < totalFrames; ++i) {
      capture >> frame;
      if (frame.empty())
        break;

      {
        std::lock_guard<std::mutex> lock(queueMutex);
        frameQueue.push(frame);
      }
      queueCV.notify_one();
    }
  });

  {
    std::lock_guard<std::mutex> lock(queueMutex);
    finishedReading = true;
  }
  queueCV.notify_all();

  for (auto& t : threads) {
    t.join();
  }

  outputVideo.release();
}
