#include <common.h>
#include <cpu.h>
#include <gpu.h>
#include <iostream>
#include "video.h"

int main(int argc, char** argv) {

  if (argc < 2) {
    std::cerr << "Usage: " << argv[0] << " <cpu/gpu> <path_to_video_file>"
              << std::endl;
    return 1;
  }

  std::string processor = argv[1];

  std::string videoFilePath = argv[2] ? argv[2] : "";
  if (videoFilePath.empty()) {
    std::cerr << argv[0] << ": File name should not be empty" << std::endl;
    return 1;
  }

  Video video;
  if (!video.loadFile(videoFilePath)) {
    std::cerr << argv[0] << ": Failed to load video!" << std::endl;
  }

  if (processor == "cpu") {
    cpu(video);
  } else if (processor == "gpu") {
    gpu::gpu(video);
  } else {
    std::cerr << argv[0] << ": Invalid option" << argv[1] << std::endl;
  }

  return 0;
}
