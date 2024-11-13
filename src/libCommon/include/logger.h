#pragma once
#include <chrono>
#include <ctime>
#include <functional>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

#define LogStream(level) utill::DestructorLogger(__FILE__, __LINE__, level)
#define TaggedLogStream(str, level) utill::DestructorLogger(__FILE__, __LINE__, str, level)

enum LOG_LEVEL {
  LEVEL_INFO = 32,
  LEVEL_LOG = 35,
  LEVEL_WARNING = 33,
  LEVEL_ERROR = 31,
  LEVEL_DEBUG = 36,
  LEVEL_VERBOSE = 39
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
namespace utill {
class DestructorLogger {
 public:
  static bool logs_enabled;
  static void disable_logs() {
    logs_enabled = false;
  }
  static void enable_logs() {
    logs_enabled = true;
  }
  static std::function<void(std::string)> log_func;
  DestructorLogger(const char* file, int line);
  DestructorLogger(const char* file, int line, std::string TAG, int color);
  DestructorLogger(const char* file, int line, int color);

  template <typename T>
  DestructorLogger& operator<<(const T& value) {
    stream << value << " ";
    return *this;
  }

  ~DestructorLogger() {
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
}  // namespace utill
