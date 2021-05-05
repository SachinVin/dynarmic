#!/bin/sh

set -e
set -x
set -o pipefail

export MACOSX_DEPLOYMENT_TARGET=11.0

mkdir build && cd build
cmake .. -GXcode -DBoost_INCLUDE_DIRS=${PWD}/../externals/ext-boost -DDYNARMIC_TESTS=0 -DCMAKE_OSX_ARCHITECTURES=arm64
xcodebuild -configuration Release
