add_library(common)
target_include_directories(common PUBLIC ./include/ include ${OpenCV_INCLUDE_DIRS})
add_subdirectory(src)