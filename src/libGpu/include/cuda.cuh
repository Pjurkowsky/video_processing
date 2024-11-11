#include <iostream>

namespace cuda {
__global__ void bgr_to_mono(uint8_t* frame, int height, int width);
__global__ void resize_kernel(uint8_t* src_frame, uint8_t* dst_frame, int src_height, int src_width, int dst_height, int dst_width);
}
