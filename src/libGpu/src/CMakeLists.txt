
if(USE_CUDA)
  target_sources(gpu PUBLIC ./gpu.cpp ./cuda.cu)
else()
  target_sources(gpu PUBLIC ./gpu_stubs.cpp)
endif()
