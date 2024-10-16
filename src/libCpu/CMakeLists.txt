add_library(cpu)

target_include_directories(cpu PUBLIC ./include/)
target_link_libraries(cpu PUBLIC common)
add_subdirectory(src)
