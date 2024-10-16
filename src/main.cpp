#include <common.h>
#include <cpu.h>
#include <gpu.h>
#include <iostream>

int main(int argc, char** argv) {

  common();
  cpu();
  gpu();
  std::cout << "Hello from the main file" << std::endl;
  return 0;
}
