#include <iostream>

__global__ void test(int a, int b, int* c) {
  *c = a + b;
}

int main() {
  std::cout << "CUDA" << std::endl;
  int a = 1;
  int b = 2;
  int c = 5;
  test<<<1, 1>>>(a, b, &c);

  std::cout << c << std::endl;
  return 0;
}