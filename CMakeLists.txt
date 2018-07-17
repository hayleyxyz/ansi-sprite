cmake_minimum_required(VERSION 3.9)
project(ansi)

set(CMAKE_VERBOSE_MAKEFILE true)

set(CMAKE_CXX_STANDARD 17)

add_executable(ansi main.cpp ansi_colours.h)

find_package(ImageMagick COMPONENTS Magick++)

target_include_directories(ansi PUBLIC ${ImageMagick_INCLUDE_DIRS})
target_compile_definitions(ansi PUBLIC -DMAGICKCORE_HDRI_ENABLE=1 -DMAGICKCORE_QUANTUM_DEPTH=16)

target_link_libraries(ansi ${ImageMagick_LIBRARIES})