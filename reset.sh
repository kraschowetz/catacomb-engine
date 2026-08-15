#!/bin/bash

if [ $# -eq 0 ]; then
    echo "reset.sh is a command line tool that resets & reconfigures cmake"
    echo "insert one of the following arguments:"
    echo "[develop | debug | release]"
fi

if [[ "$1" == "develop" ]]; then
    rm -rf ./build/develop
    cmake -B build/develop -DCMAKE_BUILD_TYPE=Develop
elif [[ "$1" == "debug" ]]; then
    rm -rf ./build/debug
    cmake -B build/debug -DCMAKE_BUILD_TYPE=Debug
elif [[ "$1" == "release" ]]; then
    rm -rf ./build/release
    cmake -B build/release -DCMAKE_BUILD_TYPE=Release
fi
