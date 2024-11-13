#include "logger.h"
#include <argparse.hpp>
#include <exception>
#include <string>

#define ERROR_LOG TaggedLogStream("Argument Parser", LEVEL_ERROR)
namespace utill {

int parse_cmd_args(int argc, char **argv, program_config &config) {

  std::vector<commandline_argument> options;
  options.push_back(
      commandline_argument{"--processor", "-p",
                           [&](std::string s, program_config &c) {
                             if (s == "CPU") {
                               c.processor = CPU;
                             } else if (s == "GPU") {
                               c.processor = GPU;
                             } else {
                               ERROR_LOG << s
                                         << " processor type is not supported. "
                                            "Available options are [CPU/GPU]";
                               return false;
                             }
                             return true;
                           },
                           true});
  options.push_back(commandline_argument{"--input", "-i",
                                         [&](std::string s, program_config &c) {
                                           c.input_path = s;
                                           return true;
                                         },
                                         true});
  options.push_back(commandline_argument{"--output", "-o",
                                         [&](std::string s, program_config &c) {
                                           c.output_path = s;
                                           return true;
                                         },
                                         true});
  options.push_back(commandline_argument{
      "--batch-size", "-b",
      [&](std::string s, program_config &c) {
        try {
          int b = std::stoi(s);
          c.batch_size = b;
        } catch (...) {
          ERROR_LOG << "Expected a number for --batch-size, but got " << s
                    << " instead";
          return false;
        }
        return true;
      },
      true});
  options.push_back(
      commandline_argument{"--encoder", "-e",
                           [&](std::string s, program_config &c) {
                             if (s == "CPU") {
                               c.encoder = CPU_ENCODER;
                             } else if (s == "GPU") {
                               c.encoder = GPU_ENCODER;
                             } else {
                               ERROR_LOG << s
                                         << " encoder type is not supported. "
                                            "Available options are [CPU/GPU]";
                               return false;
                             }
                             return true;
                           },
                           true});
  options.push_back(
      commandline_argument{"--decoder", "-d",
                           [&](std::string s, program_config &c) {
                             if (s == "CPU") {
                               c.decoder = CPU_DECODER;
                             } else if (s == "GPU") {
                               c.decoder = CPU_DECODER;
                             } else {
                               ERROR_LOG << s
                                         << " encoder type is not supported. "
                                            "Available options are [CPU/GPU]";
                               return false;
                             }
                             return true;
                           },
                           true});

  try {
    for (int i = 1; i < argc + 1; i += 2) {
      std::string arg = argv[i];
      for (int j = 0; j < options.size(); j++) {
        if (options[j].arg == arg || options[j].arg_short == arg) {
          if (options[j].f(argv[i + 1], config) != 0)
            return -1;
          options.erase(options.begin() + j, options.begin() + j + 1);
          ERROR_LOG << options.size();
          break;
        }
      }
    }
    for (int i = 0; i < options.size(); i++) {
      if (options[i].required) {
        TaggedLogStream("Argument Parser", LEVEL_ERROR)
            << "Missing required argument " << options[i].arg_short << "/"
            << options[i].arg;
        return -1;
      }
    }
  } catch (std::exception e) {
    ERROR_LOG << "An exception has occured while parsing the program arguments";
    ERROR_LOG << e.what();
    return -1;
  }
  return 0;
}
} // namespace utill
