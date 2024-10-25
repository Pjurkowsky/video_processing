add_library(gpu)
target_include_directories(gpu PUBLIC ./include/ ../common/include/)
set_target_properties(gpu PROPERTIES CUDA_ARCHITECTURES "75")
target_link_libraries(gpu PUBLIC common)


target_include_directories(gpu_main PUBLIC ./include/ ../libCommon/include/)
set_target_properties(gpu_main PROPERTIES CUDA_ARCHITECTURES "75")
target_link_libraries(gpu_main PUBLIC common gpu)

add_subdirectory(src)
