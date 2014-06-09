cmake_minimum_required(VERSION 2.8.11)

project(WinK760)

if (MINGW)
	set(CMAKE_BUILD_TYPE "Release")
	set(CMAKE_CXX_STANDARD_LIBRARIES "-lsetupapi")
ENDIF(MINGW)

# output binaries to the root of the build directory
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR} )
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR} )

set(Headers "")
set(Sources "")
set(Resources "")

add_subdirectory(lib)
add_subdirectory(res)
add_subdirectory(src)

add_definitions(-DUNICODE -D_UNICODE)

add_executable(WinK760 WIN32 ${Sources} ${Headers} ${Resources})

set_target_properties(WinK760 PROPERTIES COMPILE_DEFINITIONS "WIN32_LEAN_AND_MEAN")
set_target_properties(WinK760 PROPERTIES COMPILE_FLAGS "/W4")

target_include_directories(WinK760 PRIVATE .)
target_link_libraries(WinK760 hidapi)