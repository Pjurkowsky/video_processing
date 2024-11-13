#pragma once
#include <functional>
#include <string>

void common();

namespace utill {
void benchmark(const std::function<void()>& f);
void benchmark(std::string label, const std::function<void()>& f);
int silent_benchmark(const std::function<void()>& f);
}  // namespace utill
