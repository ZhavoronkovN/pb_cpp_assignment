cmake_minimum_required(VERSION 3.14)

project(compressor LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(compressor STATIC
  compressor.cpp
  compressor.h
  bitmap.h
)

target_compile_definitions(compressor PRIVATE COMPRESSOR_LIBRARY)
