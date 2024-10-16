add_library(gpu)

target_include_directories(gpu PUBLIC ./include/ ../common/include/)
target_link_libraries(gpu PUBLIC common)
add_subdirectory(src)
