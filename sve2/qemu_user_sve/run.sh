#!/bin/bash
set -ex
SOURCE_DIR=$(dirname $(readlink -f $0))
aarch64-none-linux-gnu-g++ ${SOURCE_DIR}/test.cpp -march=armv8-a+sve  -o test_elf --static
# export PATH=/home/xxx/download/gcc-arm-10.3-2021.07-x86_64-aarch64-none-linux-gnu/bin:$PATH
qemu-aarch64 -cpu max,sve=on,sve512=on test_elf