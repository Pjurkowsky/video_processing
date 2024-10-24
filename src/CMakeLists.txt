add_subdirectory(libCommon)

if(USE_CUDA)
    add_subdirectory(libGpu)
endif()

add_subdirectory(libCpu)

target_sources(main PUBLIC main.cpp)
