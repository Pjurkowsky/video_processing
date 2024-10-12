# Build

## Requirements

- CMake (v3.28 or above)
- Any C++ compiler supporting C++17
- Ninja
- OpenCV library (version 4.6.0)
     - For Debian based systems:
       - `sudo apt install libopencv-dev`
    - For Arch:
      - `sudo pacman -S opencv`
    -  For Windows:
       -  Download pre-built library from sourceforge https://sourceforge.net/projects/opencvlibrary/files/4.6.0/opencv-4.6.0-vc14_vc15.exe/download
       -  Run the executable, it will be a self-extracting zip archive
       -  Extract is wherever you want
       -  (optional) set an env variable OpenCV_DIR to `{path_to_extracted_archive}\build\x64\vc15\lib}`
## Building the project

- In the root of the project create a directory named `/build` (or whatever you want)
- Cd into that directory
- Run the cmake generator
  - If you are running Linux or have exported the env variable on Windows, just `cmake ..`
  - If you did not export the env variable,  `cmake .. -DOpenCV_DIR="{path_to_opencv}\build\x64\vc15\lib"`
- Compile the project `cmake --build .`
- Run the binary `./main` or `main.exe` for Linux/Windows respectively

