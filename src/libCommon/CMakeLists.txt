
find_package(OpenCV REQUIRED)

add_library(common)

target_include_directories(common PUBLIC ./include/ include ${OpenCV_INCLUDE_DIRS})
target_link_libraries(common PUBLIC ${OpenCV_LIBS})
add_subdirectory(src)
