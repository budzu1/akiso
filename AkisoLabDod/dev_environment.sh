#!/bin/bash

source distro_det.sh

$PKG_UPDATE

$PKG_INSTALL gcc make git clang cmake clang

echo 

gcc --version
echo 

make --version
echo 

git --version
echo 

clang --version
echo 

cmake --version
echo 

clang --version
echo 