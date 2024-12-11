#include "video.h"
#include <argparse.hpp>
#include <chrono>
#include <common.h>
#include <cpu.h>
#include <gpu.h>
#include <iostream>
#include <logger.h>
#include <opencv2/core/cvstd_wrapper.hpp>

#define LOG(level) TaggedLogStream("MAIN", level)

#define ERROR_LOG LOG(LEVEL_ERROR)
#define INFO_LOG LOG(LEVEL_INFO)
#define WARNING_LOG LOG(LEVEL_WARNING)

int main(int argc, char **argv) {

  utill::program_config config{.width = 640, .height = 480};
  if (utill::parse_cmd_args(argc, argv, config) != 0) {
    return 0 - 1;
    ;
  }

  Video video;
  if (!video.loadFile(config.input_path)) {
    WARNING_LOG << ": Failed to load video from " << config.input_path;
    return 1;
  }
  int time = 0;
  if (config.processor == utill::CPU) {
    time = utill::benchmark("Total processing time on CPU", [&]() {
      cpu::cpu(video, config.operation, config.batch_size, config.output_path,
               config.width, config.height);
    });
  } else if (config.processor == utill::GPU) {
    time = utill::benchmark("Total processing time on GPU", [&]() {
      gpu::gpu(video, config.operation, config.batch_size, config.output_path,
               config.width, config.height);
    });
  }
  std::string operation = config.operation == utill::MONO ? "MONO" : "RESIZE";
  std::string processor = config.processor == utill::CPU ? "CPU" : "GPU";

  std::cout << config.input_path << ";" << processor << ";" << operation << ";"
            << config.input_width << ";" << config.input_height << ";"
            << config.width << ";" << config.height << ";" << config.batch_size
            << ";" << time << std::endl;

  return 0;
}
