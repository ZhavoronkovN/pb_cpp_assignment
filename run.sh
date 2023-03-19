#!/bin/bash

CMAKE_PREFIX_PATH="$QT_CMAKE_IMPORTS" cmake -S . -B build && (cd build && make && cd ..) && ./build/PbApp