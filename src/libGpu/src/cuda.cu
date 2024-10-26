#include <common.h>
#include <cstdint>
#include <cstring>
#include <cuda.cuh>
#include <cuda.hpp>

__global__ void cuda::bgr_to_mono(uint8_t* frame, int height, int width) {
  for (int row = 0; row < height; row++) {
    for (int col = 0; col < width; col++) {
      int index = 3 * (row * width + col);
      uint8_t avg = (frame[index] + frame[index + 1] + frame[index + 2]) / 3;
      frame[index] = avg;
      frame[index + 1] = avg;
      frame[index + 2] = avg;
    }
  }
}

void gpu::bgr_to_mono(uint8_t* frame, int height, int width) {
  uint8_t* buffer;
  int buffer_size = sizeof(uint8_t) * width * height * 3;

  cudaMallocManaged(&buffer, buffer_size);
  memcpy(buffer, frame, buffer_size);
  benchmark([&buffer, &width, &height]() {
    cuda::bgr_to_mono<<<1, 1>>>(buffer, height, width);
  });
  cudaDeviceSynchronize();

  memcpy(frame, buffer, buffer_size);
  cudaFree(buffer);
}
