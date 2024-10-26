#include <common.h>
#include <chrono>
#include <functional>
#include <iostream>

void common() {
  std::cout << "Hello from the Common library" << std::endl;
}

void benchmark(const std::function<void()>& f) {
  auto start = std::chrono::steady_clock::now();
  f();
  auto end = std::chrono::steady_clock::now();
  std::cout << (end - start) / std::chrono::milliseconds(1) << " ms"
            << std::endl;
  ;
}
