#include <common.h>
#include <chrono>
#include <functional>
#include <iostream>

void common() {
  std::cout << "Hello from the Common library" << std::endl;
}

void utill::benchmark(const std::function<void()>& f) {
  auto start = std::chrono::steady_clock::now();
  f();
  auto end = std::chrono::steady_clock::now();
  std::cout << ((float)((end - start) / std::chrono::microseconds(1))) / 1000.0f
            << " ms" << std::endl;
  ;
}

void utill::benchmark(std::string label, const std::function<void()>& f) {
  std::cout << label << " ";
  benchmark(f);
  ;
}
