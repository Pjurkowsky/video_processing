
#include <cstdint>
#include <opencv2/cudacodec.hpp>

namespace gpu {

void bgr_to_mono(uint8_t *frame, int height, int width);
void bgr_to_mono(uint8_t *frames, int frame_count, int height, int width,
                 uint8_t *buffer);
void malloc_memory(uint8_t **buffer, int buffer_size);
void free_memory(uint8_t *buffer);
void resize(uint8_t *frames, int batch_size, int src_height, int src_width,
            int dst_height, int dst_width, uint8_t *resized_frames,
            uint8_t *src_buffer, uint8_t *dst_buffer);
void copyDeviceDevice(uint8_t *source, uint8_t *target, int size);
void init();

} // namespace gpu
