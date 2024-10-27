#include <common.h>
#include <cstdint>
#include <cstring>
#include <cuda.cuh>
#include <cuda.hpp>

__global__ void cuda::bgr_to_mono(uint8_t* frame, int height, int width) {
  int index = blockIdx.x * blockDim.x + threadIdx.x;
  int stride = blockDim.x * gridDim.x;
  for (int row = index; row < height; row += stride) {
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

  cudaMalloc(&buffer, buffer_size);
  cudaMemcpy(buffer, frame, buffer_size, cudaMemcpyKind::cudaMemcpyHostToDevice);
  cuda::bgr_to_mono<<<16, 256>>>(buffer, height, width);
  cudaDeviceSynchronize();
  cudaMemcpy(frame, buffer, buffer_size, cudaMemcpyKind::cudaMemcpyDeviceToHost);
  cudaFree(buffer);
}

void gpu::bgr_to_mono(uint8_t* frames, int frame_count, int height, int width) {

  uint8_t* buffer;
  int frame_size = sizeof(uint8_t) * width * height * 3;
  cudaMalloc(&buffer, frame_size);

  for (int i = 0; i < 1; i++) {
    cudaMemcpy(buffer, frames + frame_size * i, frame_size, cudaMemcpyKind::cudaMemcpyHostToDevice);
    cuda::bgr_to_mono<<<16, 256>>>(buffer, height, width);
    cudaDeviceSynchronize();
    cudaMemcpy(frames + frame_size * i, buffer, frame_size, cudaMemcpyKind::cudaMemcpyDeviceToHost);
  }

  cudaFree(buffer);
}

void gpu::init() {
  cudaFree(0);
}
