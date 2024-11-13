#include "logger.h"

#include <iomanip>

namespace utill {
bool DestructorLogger::logs_enabled = true;
std::function<void(std::string)> DestructorLogger::log_func = [](std::string s) {
  std::cerr << s;
};

DestructorLogger::DestructorLogger(const char* file, int line) {
  std::string str{file};

  str = str.substr(str.find_last_of('/') + 1);
  auto now = std::chrono::system_clock::now();
  time_t t = std::chrono::system_clock::to_time_t(now);
  stream << "\e[1;90m";
  stream << "[" << std::put_time(localtime(&t), "%H:%M:%S");
  stream << "]" << std::setw(20) << std::left << str + ":" + std::to_string(line) << " ";
  stream << "\e[0m";
}
DestructorLogger::DestructorLogger(const char* file, int line, std::string TAG, int color) {
  std::string str{file};

  str = str.substr(str.find_last_of('/') + 1);
  auto now = std::chrono::system_clock::now();
  time_t t = std::chrono::system_clock::to_time_t(now);
  stream << "\e[1;90m";
  stream << "[" << std::put_time(localtime(&t), "%H:%M:%S") << "] ";
  stream << "\e[0m";

  stream << "\e[1;" << color << "m";
  stream << std::left << std::setw(15) << str + ":" + std::to_string(line) << " ";
  stream << std::left << "[" << TAG << "] ";
}

DestructorLogger::DestructorLogger(const char* file, int line, int color) {
  std::string str{file};

  str = str.substr(str.find_last_of('/') + 1);
  auto now = std::chrono::system_clock::now();
  time_t t = std::chrono::system_clock::to_time_t(now);
  stream << "\e[1;90m";
  stream << "[" << std::put_time(localtime(&t), "%H:%M:%S") << "] ";
  stream << "\e[0m";

  stream << "\e[1;" << color << "m";
  stream << std::left << std::setw(15) << str + ":" + std::to_string(line) << " ";
}
}  // namespace utill
