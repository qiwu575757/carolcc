arm-linux-gnueabi-gcc test.s -static -c
arm-linux-gnueabi-gcc test.o stdlib/libsysy_no_float.a -static
qemu-arm a.out
echo $?

#  1811  arm-linux-gnueabi-gcc -c sylib.c -static
#  1812  arm-linux-gnueabi-ar rcs libsysy_no_float.a sylib.o