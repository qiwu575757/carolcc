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

# scp -P 5693 build/test_results/median2.sy/arm_asm/npu_npu.s  pi@47.100.255.80:/home/pi/Documents/qiwu
# ssh pi@47.100.255.80 -p 5693
# gcc -g crypto-1.c sylib.c sylib.h -o crypto-test
# gcc -g crypto-1.s sylib.c -o crypto-1
# gcc sl1.c sylib.c sylib.h -S sl1.s