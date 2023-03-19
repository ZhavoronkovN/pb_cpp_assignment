cmake_minimum_required(VERSION 3.14)

project(compressor LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
include_directories(CImg)

add_library(compressor compressor.cpp compressor.h bitmap.cpp bitmap.h)

target_link_libraries( compressor "-lX11" )
target_include_directories(compressor
                           INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
                           )