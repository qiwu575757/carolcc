arm-linux-gnueabihf-gcc test.s -static -c
arm-linux-gnueabihf-gcc test.o stdlib/libsysy_float.a -static
# arm-linux-gnueabi-gcc test.s -static -c
# arm-linux-gnueabi-gcc test.o stdlib/libsysy_no_float.a -static
qemu-arm a.out
echo $?

#  1811  arm-linux-gnueabi-gcc -c sylib.c -static
#  1812  arm-linux-gnueabi-ar rcs libsysy_no_float.a sylib.o

# arm-linux-gnueabihf-gcc -c sylib.c -static
# arm-linux-gnueabihf-ar rcs libsysy_float.a sylib.o