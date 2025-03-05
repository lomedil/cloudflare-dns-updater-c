#!/bin/bash

THIS_DIR=$(realpath "$0" | xargs dirname)
TEST_BIN=$THIS_DIR/build/main/test/test

cd $THIS_DIR/build/

cmake --build .
if [ $? -eq 0 ]; then
    $TEST_BIN
else
    echo "Build failed"
    exit 1
fi

exit 0