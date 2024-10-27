
#include <cstdint>

namespace gpu {

void bgr_to_mono(uint8_t* frame, int height, int width);
void bgr_to_mono(uint8_t* frames, int frame_count, int height, int width);
void init();

}  // namespace gpu
