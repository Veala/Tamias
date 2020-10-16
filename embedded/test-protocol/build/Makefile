cs:  ../test.c ../chip.c ../file.c ../cs.h
	arm-linux-gnueabi-gcc  ../test.c ../chip.c ../file.c -o cs4
ser: ../server.c
	arm-linux-gnueabi-gcc ../server.c -o ser
client: ../server.c
	arm-linux-gnueabi-gcc ../client.c -o client
clear:
	rm -rf ser* *~
	rm -rf c* *~

