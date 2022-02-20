#!/bin/bash
set -ex
qemu-system-aarch64 -machine virt,virtualization=on -m 1024M -cpu max,sve=on,sve512=on -nographic -s -kernel test_elf -semihosting