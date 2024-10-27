
#include <common.h>
#include <gpu.h>
#include <logger.h>
#include <opencv2/core/hal/interface.h>
#include <cstdlib>
#include <cstring>

void gpu::gpu(Video video) {
  LogTag("GPU STUB", WARNING) << "Calls to libGpu without USE_CUDA result in no-ops";
}
