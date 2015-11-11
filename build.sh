#!/bin/sh
#
# Shell Script to build cg01-intersection
#

echo "Creating build/ directory ..."
mkdir -p build
cd build
cmake ..
make
./cg01_intersection