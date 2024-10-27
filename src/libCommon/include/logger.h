#pragma once
#include <chrono>
#include <ctime>
#include <functional>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

#define Log(level) Logger(__FILE__, __LINE__, level)
#define LogTag(str, color) Logger(__FILE__, __LINE__, str, color)

enum LOG_LEVEL {
  INFO = 32,
  LOG = 35,
  WARNING = 33,
  ERROR = 31,
  DEBUG = 36,
  VERBOSE = 39
};

/* *
 * @brief Logger class to use instead of std::cout
 *
 * @remark Do NOT use this class directly, instead use the 2 macros
 * above, Log(level) and LogTag(level)
 *
 * Example usage:
 *  Log(INFO) << "My awesome log message";
 *  Log("MODULE_NAME", DEBUG) << "Even more awesome log message (with a tag this time)"
 *
 * To avoid writing the tag multiple times, you can define per-file loggers with 
 * #define LOG(level) LogTag("TAG_FOR_A_FILE",level) at the top of the file (don't use in headers pls)
 * and use as 
 * LOG(INFO) << "Message"
 *
 */
class Logger {
 public:
  static bool logs_enabled;
  static void disable_logs() {
    logs_enabled = false;
  }
  static void enable_logs() {
    logs_enabled = true;
  }
  static std::function<void(std::string)> log_func;
  Logger(const char* file, int line);
  Logger(const char* file, int line, std::string TAG, int color);
  Logger(const char* file, int line, int color);

  template <typename T>
  Logger& operator<<(const T& value) {
    stream << value << " ";
    return *this;
  }

  ~Logger() {
    stream << std::endl;
    if (logs_enabled) {
      stream << "\e[0m";
      log_func(stream.str());
      ;
    }
  }

 private:
  std::ostringstream stream;
};
