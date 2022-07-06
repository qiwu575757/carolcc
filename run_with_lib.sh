arm-linux-gnueabi-gcc test.s -static -c
arm-linux-gnueabi-gcc test.o stdlib/libsysy_no_float.a -static
qemu-arm a.out
echo $?