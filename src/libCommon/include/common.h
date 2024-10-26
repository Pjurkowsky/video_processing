#pragma once
#include <chrono>
#include <functional>
#include "video.h"

void common();

void benchmark(const std::function<void()>& f);
