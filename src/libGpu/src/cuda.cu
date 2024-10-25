#include <cuda.cuh>

__global__ void add(float* a, float* b) {
  *a = *a + *b;
}

float add_gpu(float a, float b) {
  float *x, *y;
  float res = 0.0;
  cudaMallocManaged(&x, sizeof(float));
  cudaMallocManaged(&y, sizeof(float));
  *x = a;
  *y = b;
  add<<<1, 1>>>(x, y);
  cudaDeviceSynchronize();
  res = *x;
  cudaFree(x);
  cudaFree(y);
  return res;
}
