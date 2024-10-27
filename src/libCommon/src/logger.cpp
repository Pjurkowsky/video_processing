#include "logger.h"

#include <iomanip>

bool Logger::logs_enabled = true;
std::function<void(std::string)> Logger::log_func = [](std::string s) {
  std::cerr << s;
};

Logger::Logger(const char* file, int line) {
  std::string str{file};

  str = str.substr(str.find_last_of('/') + 1);
  auto now = std::chrono::system_clock::now();
  time_t t = std::chrono::system_clock::to_time_t(now);
  stream << "\e[1;90m";
  stream << "[" << std::put_time(localtime(&t), "%H:%M:%S");
  stream << "]" << std::setw(20) << std::left << str + ":" + std::to_string(line) << " ";
  stream << "\e[0m";
}
Logger::Logger(const char* file, int line, std::string TAG, int color) {
  std::string str{file};

  str = str.substr(str.find_last_of('/') + 1);
  auto now = std::chrono::system_clock::now();
  time_t t = std::chrono::system_clock::to_time_t(now);
  stream << "\e[1;90m";
  stream << "[" << std::put_time(localtime(&t), "%H:%M:%S") << "] ";
  stream << "\e[0m";

  stream << "\e[1;" << color << "m";
  stream << std::left << str + ":" + std::to_string(line) << " ";
  stream << std::left << "[" << TAG << "] ";
}

Logger::Logger(const char* file, int line, int color) {
  std::string str{file};

  str = str.substr(str.find_last_of('/') + 1);
  auto now = std::chrono::system_clock::now();
  time_t t = std::chrono::system_clock::to_time_t(now);
  stream << "\e[1;90m";
  stream << "[" << std::put_time(localtime(&t), "%H:%M:%S") << "] ";
  stream << "\e[0m";

  stream << "\e[1;" << color << "m";
  stream << std::left << str + ":" + std::to_string(line) << " ";
}
