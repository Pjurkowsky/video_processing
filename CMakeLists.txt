cmake_minimum_required(VERSION 3.28)

project(main)

find_package(OpenCV REQUIRED)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)
add_executable(main)
target_link_libraries(main ${OpenCV_LIBS} common gpu cpu)

add_subdirectory(src)
target_include_directories(
  main PUBLIC ${OpenCV_INCLUDE_DIRS} libCommon/include/ libCpu/include/
              libGpu/include/)
