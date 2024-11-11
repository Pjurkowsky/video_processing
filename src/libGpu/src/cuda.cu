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

void gpu::bgr_to_mono(uint8_t* frames, int batch_size, int height, int width) {
  uint8_t* buffer;
  int frame_size = sizeof(uint8_t) * width * height * 3;
  int total_size = frame_size * batch_size;
  cudaMalloc(&buffer, total_size);

    cudaMemcpy(buffer, frames, total_size, cudaMemcpyKind::cudaMemcpyHostToDevice);
    for (int i = 0; i < batch_size; i++) {
      uint8_t* frame_ptr = buffer + i * frame_size;
      cuda::bgr_to_mono<<<16, 256>>>(frame_ptr, height, width);
    }
    cudaDeviceSynchronize();
    cudaMemcpy(frames, buffer, total_size, cudaMemcpyKind::cudaMemcpyDeviceToHost);
  
  cudaFree(buffer);
}

void gpu::resize(uint8_t* frames, int batch_size, int src_height, int src_width, int dst_height, int dst_width, uint8_t* resized_frames) {
    uint8_t* src_buffer;
    uint8_t* dst_buffer;
    int frame_size = sizeof(uint8_t) * src_width * src_height * 3;
    int resized_frame_size = sizeof(uint8_t) * dst_width * dst_height * 3;
    int total_size = frame_size * batch_size;

    cudaMalloc(&src_buffer, total_size);
    cudaMalloc(&dst_buffer, resized_frame_size * batch_size);

    cudaMemcpy(src_buffer, frames, total_size, cudaMemcpyHostToDevice);

    for (int i = 0; i < batch_size; i++) {
        uint8_t* src_frame = src_buffer + i * frame_size;
        uint8_t* dst_frame = dst_buffer + i * resized_frame_size;
        dim3 block(16, 16);
        dim3 grid((dst_width + 15) / 16, (dst_height + 15) / 16);
        cuda::resize_kernel<<<grid, block>>>(src_frame, dst_frame, src_height, src_width, dst_height, dst_width);
    }

    cudaDeviceSynchronize();

    cudaMemcpy(resized_frames, dst_buffer, resized_frame_size * batch_size, cudaMemcpyDeviceToHost);

    cudaFree(src_buffer);
    cudaFree(dst_buffer);
}

// nearest neighbour algorithm
__global__ void cuda::resize_kernel(uint8_t* src_frame, uint8_t* dst_frame, int src_height, int src_width, int dst_height, int dst_width) {
    int i = blockDim.y * blockIdx.y + threadIdx.y;
    int j = blockDim.x * blockIdx.x + threadIdx.x;

    int channel = 3;

    if (i < dst_height && j < dst_width) {
        int iIn = i * src_height / dst_height;
        int jIn = j * src_width / dst_width;

        dst_frame[(i * dst_width + j) * channel + 0] = src_frame[(iIn * src_width + jIn) * channel + 0];
        dst_frame[(i * dst_width + j) * channel + 1] = src_frame[(iIn * src_width + jIn) * channel + 1];
        dst_frame[(i * dst_width + j) * channel + 2] = src_frame[(iIn * src_width + jIn) * channel + 2];
    }
}

void gpu::init() {
  cudaFree(0);
}
