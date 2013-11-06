cmake_minimum_required(VERSION 2.8.11)

project(WinK760)

set(CMAKE_PREFIX_PATH "C:/Qt/5.1.1/msvc2012_64_opengl" "C:/Program Files (x86)/Windows Kits/8.0/Lib/win8/um/x64")

# find qt packages
find_package(Qt5Core REQUIRED)
find_package(Qt5Qml REQUIRED)
find_package(Qt5Quick REQUIRED)

# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

# As moc files are generated in the binary dir, tell CMake
# to always look for includes there:
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_BUILD_TYPE "Release")
set(CMAKE_CXX_STANDARD_LIBRARIES "-lsetupapi")

set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR} )
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR} )



# file(GLOB_RECURSE WinK760_QRC "res/*.qrc")

qt5_add_resources(WinK760_QRC "res/resources.qrc") 

message(STATUS "QRC " ${WinK760_QRC})

# src/hid.c src/hidapi.h
add_executable(${CMAKE_PROJECT_NAME} src/main.cpp ${WinK760_QRC})

target_link_libraries(${CMAKE_PROJECT_NAME} Qt5::Qml Qt5::Quick)