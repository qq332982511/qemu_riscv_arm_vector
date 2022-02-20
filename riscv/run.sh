#!/bin/bash
set -ex
SOURCE_DIR=$(dirname $(readlink -f $0))
# export PATH=/home/xxx/download/thead/Xuantie-900-gcc-elf-newlib-x86_64-V2.2.4/bin:$PATH
riscv64-unknown-elf-g++ ${SOURCE_DIR}/test.cpp -o test_app -march=rv64gcv0p7
qemu-riscv64  -cpu rv64,x-v=true,vlen=256,elen=64 ./test_app