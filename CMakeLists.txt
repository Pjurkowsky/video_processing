cmake_minimum_required(VERSION 3.28)

project(main)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
message(${OpenCV_INCLUDE_DIRS})
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
add_executable(main src/main.cpp)

target_link_libraries(main ${OpenCV_LIBS})
