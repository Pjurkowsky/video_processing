add_subdirectory(libCommon)
add_subdirectory(libGpu)
add_subdirectory(libCpu)

target_sources(main PUBLIC main.cpp)
