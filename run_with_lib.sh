arm-linux-gnueabihf-gcc test.s test.o -static -c
arm-linux-gnueabihf-gcc test.o stdlib/libsysy_float.a -static
# arm-linux-gnueabi-gcc test.s -static -c
# arm-linux-gnueabi-gcc test.o stdlib/libsysy_no_float.a -static
qemu-arm a.out && echo -e
echo $?

#  1811  arm-linux-gnueabi-gcc -c sylib.c -static
#  1812  arm-linux-gnueabi-ar rcs libsysy_no_float.a sylib.o

# arm-linux-gnueabihf-gcc -c sylib.c -static
# arm-linux-gnueabihf-ar rcs libsysy_float.a sylib.o

#连接树莓派，cp文件
# scp ./test.s pi@128g_pi:/home/pi/Documents/qiwu
# ssh pi@128g_pi
# cd /home/pi/Documents/qiwu
# ./run.sh
# logout
