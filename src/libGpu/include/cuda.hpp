
#include <cstdint>

namespace gpu {

void bgr_to_mono(uint8_t* frame, int height, int width);
void bgr_to_mono(uint8_t* frames, int frame_count, int height, int width);
void resize(uint8_t* frames, int batch_size, int src_height, int src_width, int dst_height, int dst_width, uint8_t* resized_frames);
void init();

}  // namespace gpu
