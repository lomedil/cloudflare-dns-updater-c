#!/bin/bash

THIS_DIR=$(realpath "$0" | xargs dirname)
ROOT_DIR=$(realpath "$THIS_DIR/..")
CLI_BIN=$ROOT_DIR/build/main/cli/cfcli

cd $ROOT_DIR/build/

cmake --build .
if [ $? -eq 0 ]; then
    $CLI_BIN $*
else
    echo "Build failed"
    exit 1
fi

exit 0
