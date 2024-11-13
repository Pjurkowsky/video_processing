#include <common.h>
#include <cpu.h>
#include <gpu.h>
#include <logger.h>
#include <chrono>
#include <iostream>
#include "video.h"

#define LOG(level) TaggedLogStream("MAIN", level)

int main(int argc, char** argv) {

  if (argc < 2) {
    LOG(LEVEL_ERROR) << "Usage: " << argv[1] << " <cpu/gpu/both> <path_to_video_file>";
    return 1;
  }

  std::string processor = argv[1];

  std::string videoFilePath = argv[2] ? argv[2] : "";
  if (videoFilePath.empty()) {
    LOG(LEVEL_ERROR) << argv[0] << ": File name should not be empty";
    return 1;
  }

  Video video;
  if (!video.loadFile(videoFilePath)) {
    LOG(LEVEL_ERROR) << argv[0] << ": Failed to load video from " << argv[2];
    return 1;
  }
  if (processor == "cpu") {
    if (argc < 4) {
      LOG(LEVEL_ERROR) << "Usage: <cpu/gpu> <path_to_video_file> <filter>";
      return 1;
    }
    std::string filter = argv[3];
    utill::benchmark("CPU", [&]() {
      cpu(video, filter);
    });
  } else if (processor == "gpu") {
    if (argc < 4) {
      LOG(LEVEL_ERROR) << "Usage: " << argv[1] << " <cpu/gpu/both> <path_to_video_file> <operation> <batch_size>";
      return 1;
    }
    std::string operation = argv[3];
    int batch_size = std::stoi(argv[4]);
    utill::benchmark("CPU", [&]() {
      gpu::gpu(video, operation, batch_size);
    });
  } else {
    LOG(LEVEL_ERROR) << argv[0] << ": Invalid option" << argv[1];
    return 1;
  }
  return 0;
}
