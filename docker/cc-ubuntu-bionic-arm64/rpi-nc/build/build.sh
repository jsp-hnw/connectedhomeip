#!/usr/bin/env bash
rm -rf .
cmake -DCMAKE_TOOLCHAIN_FILE=../pi.cmake -DCMAKE_BUILD_TYPE=Debug ..
make
