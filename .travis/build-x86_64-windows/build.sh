#!/bin/sh

set -e
set -x

mkdir build && cd build
cmake .. -DBoost_INCLUDE_DIRS=${PWD}/../externals/ext-boost -DCMAKE_BUILD_TYPE=Release -G Ninja
ninja

./tests/dynarmic_tests --durations yes
