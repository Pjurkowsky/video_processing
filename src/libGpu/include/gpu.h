#include <common.h>
#include <logger.h>
#include <video.h>
#include <cuda.hpp>

namespace gpu {

void gpu(Video video);
void gpu(Video video, std::string operation, int batch_size);

}  // namespace gpu
