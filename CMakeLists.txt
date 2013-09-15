project(WinK760)

set(CMAKE_BUILD_TYPE "Release")
set(CMAKE_CXX_STANDARD_LIBRARIES "-lsetupapi")

add_executable(WinK760 src/WinK760.cpp src/hid.c src/hidapi.h)
