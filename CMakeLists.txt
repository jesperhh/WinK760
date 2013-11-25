cmake_minimum_required(VERSION 2.8.11)

project(WinK760)

# add qt and windows libraries to the path
set(CMAKE_PREFIX_PATH "C:/Qt/5.1.1/msvc2012_64_opengl" "C:/Program Files (x86)/Windows Kits/8.0/Lib/win8/um/x64")

# find qt packages
find_package(Qt5Core REQUIRED)
find_package(Qt5Qml REQUIRED)
find_package(Qt5Quick REQUIRED)
find_package(Qt5Concurrent REQUIRED)

# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

# As moc files are generated in the binary dir, tell CMake
# to always look for includes there:
set(CMAKE_INCLUDE_CURRENT_DIR ON)

if (MINGW)
	set(CMAKE_BUILD_TYPE "Release")
	set(CMAKE_CXX_STANDARD_LIBRARIES "-lsetupapi")
ENDIF(MINGW)

# output binaries to the root of the build directory
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR} )
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR} )

# SET (Sources "src/qt/ApplicationContext.cpp" "src/qt/KeyboardEventFilter.cpp" "src/hidapi/hidapi.c" "src/hid/hid.cpp" "src/main.cpp")
file(GLOB_RECURSE Sources "src/*.cpp" "src/*.c")
file(GLOB_RECURSE Headers "src/*.h")
file(GLOB_RECURSE Resources "res/*.qrc")

qt5_add_resources(Resources ${Resources}) 

# src/hid.c src/hidapi.h
add_executable(${CMAKE_PROJECT_NAME} ${Sources} ${Headers} ${Resources})

target_link_libraries(${CMAKE_PROJECT_NAME} Qt5::Qml Qt5::Quick Qt5::Concurrent)
