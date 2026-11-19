#!/bin/bash

if [ $# -eq 0 ]; then
    echo "run.sh is a command line tool that runs the sandbox binary"
    echo "insert one of the following arguments:"
    echo "[develop | debug | release]"
fi

if [[ "$1" == "develop" ]]; then
    ./build/develop/sandbox/main
elif [[ "$1" == "debug" ]]; then
    ./build/debug/sandbox/main
elif [[ "$1" == "release" ]]; then
    ./build/release/sandbox/main
fi
