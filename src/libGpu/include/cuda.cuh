#include <iostream>

namespace cuda {
__global__ void bgr_to_mono(uint8_t* frame, int height, int width);
}
