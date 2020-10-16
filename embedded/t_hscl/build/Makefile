hscl.a: hscl.o register.o
	arm-linux-gnueabi-ar cr hscl.a hscl.o register.o
hscl.o: ../src/hscl.c
	arm-linux-gnueabi-gcc -c -I../export/ -I../src/ ../src/hscl.c
register.o: ../src/register.c
	arm-linux-gnueabi-gcc -c -I../export/ -I../src/ ../src/register.c
clear:
	rm -rf *.[ao]
