#include <common.h>
#include <logger.h>
#include <chrono>
#include <exception>
#include <functional>
#include <iostream>

#define LOG(level) TaggedLogStream("BENCHMARK", level)

#define ERROR_LOG LOG(LEVEL_ERROR)
#define INFO_LOG LOG(LEVEL_INFO)
#define WARNING_LOG LOG(LEVEL_WARNING)

void common() {
  std::cout << "Hello from the Common library" << std::endl;
}

int utill::benchmark(const std::function<void()>& f) {
  return benchmark("", f);
}

int utill::benchmark(std::string label, const std::function<void()>& f) {
  auto start = std::chrono::steady_clock::now();
  try {
    f();
  } catch (...) {
    ERROR_LOG << "During a benchmark an exception has occured";
    throw std::current_exception();
  }
  auto end = std::chrono::steady_clock::now();
  TaggedLogStream(label, LEVEL_LOG) << ((float)((end - start) / std::chrono::microseconds(1))) / 1000.0f << " ms";
  return (end - start) / std::chrono::milliseconds(1);
}

int utill::silent_benchmark(const std::function<void()>& f) {
  auto start = std::chrono::steady_clock::now();
  try {
    f();
  } catch (...) {
    ERROR_LOG << "During a benchmark an exception has occured";
    throw std::current_exception();
  }
  auto end = std::chrono::steady_clock::now();
  return (end - start) / std::chrono::milliseconds(1);
}
