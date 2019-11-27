cmake_minimum_required(VERSION 3.15)

cmake_policy(SET CMP0091 NEW)

project(WinK760)

# output binaries to the root of the build directory
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR} )
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR} )

set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

add_subdirectory(lib)
add_subdirectory(res)
add_subdirectory(src)

# Sources, headers and resources from src/res subfolder
add_executable(WinK760 WIN32 ${Sources} ${Headers} ${Resources})

set_target_properties(WinK760 PROPERTIES COMPILE_DEFINITIONS "WIN32_LEAN_AND_MEAN;WIN32_EXTRA_LEAN;_UNICODE;UNICODE;_CRT_SECURE_NO_WARNINGS")

set_source_files_properties("src/main.cpp" PROPERTIES COMPILE_FLAGS "/Ycstdafx.h")
set_target_properties(WinK760 PROPERTIES COMPILE_FLAGS "/Yustdafx.h -W4 -WX")

target_include_directories(WinK760 PRIVATE .)
target_link_libraries(WinK760 hidapi)