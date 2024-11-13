#pragma once
#include <functional>
#include <string>

void common();

namespace utill {
int benchmark(const std::function<void()>& f);
int benchmark(std::string label, const std::function<void()>& f);
int silent_benchmark(const std::function<void()>& f);
}  // namespace utill
