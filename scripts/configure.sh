#!/bin/bash

THIS_DIR=$(realpath "$0" | xargs dirname)
ROOT_DIR=$(realpath "$THIS_DIR/..")

cd $ROOT_DIR

[[ -d "build" ]] && rm -rf build

if [ ! -d "build" ]; then
    mkdir build
fi

cd build

cmake ..
if [ $? -eq 0 ]; then
    cmake --build .
else
    echo "CMake failed"
    exit 1
fi