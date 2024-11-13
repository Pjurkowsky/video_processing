#include "video.h"
#include <argparse.hpp>
#include <chrono>
#include <common.h>
#include <cpu.h>
#include <gpu.h>
#include <iostream>
#include <logger.h>

#define LOG(level) TaggedLogStream("MAIN", level)

#define ERROR_LOG LOG(LEVEL_ERROR)
#define INFO_LOG LOG(LEVEL_INFO)
#define WARNING_LOG LOG(LEVEL_WARNING)

int main(int argc, char **argv) {

  if (argc < 2) {
    WARNING_LOG << "Usage: " << argv[1]
                << " <cpu/gpu/both> <path_to_video_file>";
    return 1;
  }

  // utill::program_config config{};
  // if (utill::parse_cmd_args(argc, argv, config) != 0) {
  //   ERROR_LOG << "Configuration failed. See above logs for details";
  //   return -1;
  // }
  //
  std::string processor = argv[1];

  std::string videoFilePath = argv[2] ? argv[2] : "";
  if (videoFilePath.empty()) {
    WARNING_LOG << argv[0] << ": File name should not be empty";
    return 1;
  }

  Video video;
  if (!video.loadFile(videoFilePath)) {
    WARNING_LOG << argv[0] << ": Failed to load video from " << argv[2];
    return 1;
  }
  if (processor == "cpu") {
    if (argc < 4) {
      WARNING_LOG << "Usage: <cpu/gpu> <path_to_video_file> <filter>";
      return 1;
    }
    std::string filter = argv[3];
    int batch_size = std::stoi(argv[4]);
    utill::benchmark("Total processing time on CPU", [&]() {
      int batch_size = std::stoi(argv[4]);
      cpu(video, filter, batch_size);
    });
  } else if (processor == "gpu") {
    if (argc < 4) {
      WARNING_LOG
          << "Usage: " << argv[1]
          << " <cpu/gpu/both> <path_to_video_file> <operation> <batch_size>";
      return 1;
    }
    std::string operation = argv[3];
    int batch_size = std::stoi(argv[4]);
    utill::benchmark("Total processing time on GPU",
                     [&]() { gpu::gpu(video, operation, batch_size); });
  } else {
    ERROR_LOG << argv[0] << ": Invalid option" << argv[1];
    return 1;
  }
  return 0;
}
