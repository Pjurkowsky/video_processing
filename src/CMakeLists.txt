add_subdirectory(libCommon)
add_subdirectory(libCpu)
add_subdirectory(libGpu)

target_sources(main PUBLIC main.cpp)
