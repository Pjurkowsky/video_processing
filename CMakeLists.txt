cmake_minimum_required(VERSION 3.22)

if(USE_CUDA)
  project(main LANGUAGES CXX CUDA)
  message("Building for CPU and GPU")
else()
  project(main LANGUAGES CXX)
  message("Building for CPU only")
endif()

find_package(OpenCV REQUIRED)
set(main CMAKE_EXPORT_COMPILE_COMMANDS 1)
set(main CXX_STNDARD 20)
add_executable(main)
add_executable(cpu_main ./src/libCpu/src/main.cpp)

if(USE_CUDA)
  add_executable(gpu_main ./src/libGpu/src/main.cpp)
  set_target_properties(gpu_main PROPERTIES CUDA_ARCHITECTURES "75")
  target_link_libraries(main ${OpenCV_LIBS} common gpu cpu)
  target_include_directories(main PUBLIC ${OpenCV_INCLUDE_DIRS} libCommon/include/ libCpu/include/ libGpu/include/)
else()
  target_link_libraries(main ${OpenCV_LIBS} common cpu)
  target_include_directories(main PUBLIC ${OpenCV_INCLUDE_DIRS} libCommon/include/ libCpu/include/)
endif()



add_subdirectory(src)
