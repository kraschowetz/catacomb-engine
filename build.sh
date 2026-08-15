#!/bin/bash

if [ $# -eq 0 ]; then
    echo "build.sh is a wrapper for the cmake build command"
    echo "insert one of the following argumens:"
    echo "[develop | debug | release]"
    exit 0
fi

if [[ "$1" == "develop" ]]; then
    cmake --build build/develop
elif [[ "$1" == "debug" ]]; then
    cmake --build build/debug
elif [[ "$1" == "release" ]]; then
    cmake --build build/release
fi
