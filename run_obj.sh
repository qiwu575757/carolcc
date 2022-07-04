#!/bin/bash
arm-linux-gnueabi-gcc test.s -static
qemu-arm a.out
echo $?
