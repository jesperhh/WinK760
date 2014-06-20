cmake_minimum_required(VERSION 2.8.11)

project(WinK760)

# output binaries to the root of the build directory
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR} )
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR} )

if (MSVC)
	set (CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -MTd")
    set (CMAKE_CXX_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -MTd")
    set (CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -MT")
    set (CMAKE_CXX_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -MT")
elseif (MINGW)
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -static-libgcc")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libgcc -static-libstdc++")
	set(CMAKE_BUILD_TYPE "Release")
	set(CMAKE_CXX_STANDARD_LIBRARIES "${CMAKE_CXX_STANDARD_LIBRARIES} -lsetupapi -lbthprops -lcomctl32")
ENDIF(MSVC)

add_subdirectory(lib)
add_subdirectory(res)
add_subdirectory(src)

# Sources, headers and resources from src/res subfolder
add_executable(WinK760 WIN32 ${Sources} ${Headers} ${Resources})

set_target_properties(WinK760 PROPERTIES COMPILE_DEFINITIONS "WIN32_LEAN_AND_MEAN;WIN32_EXTRA_LEAN;_UNICODE;UNICODE;_CRT_SECURE_NO_WARNINGS")

if (MSVC)
    set_source_files_properties("src/main.cpp" PROPERTIES COMPILE_FLAGS "/Ycstdafx.h")
	set_target_properties(WinK760 PROPERTIES COMPILE_FLAGS "/Yustdafx.h -W4 -WX")
elseif (MINGW)
	set_target_properties(WinK760 PROPERTIES COMPILE_FLAGS "-std=gnu++0x -Wall -Wextra -Werror -pedantic -Wshadow -Wpointer-arith -Wcast-qual -Wcast-align -Wno-parentheses")
ENDIF(MSVC)

target_include_directories(WinK760 PRIVATE .)
target_link_libraries(WinK760 hidapi)