/*#include <stdlib.h>*/
#include <stdio.h>
#include <string.h>
#include "cs.h"


#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

/*int readData(int &n, size_t &r, int& size, char (&refArray)[SIZE], timeval* tv) {
    FD_ZERO(&rfds);
    FD_SET(tcp_socket, &rfds);
    int retval = select(tcp_socket+1, &rfds, NULL, NULL, tv);
    if (retval) {
        if (FD_ISSET(tcp_socket,&rfds)) {
            r = read(tcp_socket, &refArray[n], size - n);
            if (r == -1) {
                perror("read");
                return ERROR_READ;
            }
            if (r == 0) {
                perror("connection");
                return ERROR_CONNECTION;
            }
            #ifdef debug
            printf("recv: %ld bytes\n", r);
            #endif
            return NO_ERROR;
        }
    } else if(retval == -1) {
        handle_error("select");
    } else {
        FD_ZERO(&rfds);
        printf("No data within five seconds.\n");
        return ERROR_TIME;
    }
}

*/

////////////////////////////////////////////////////////////////////////////

int onLoad(char *parameters, int length, char *error_message)
{
    return commandNotImplemented("chip:load", error_message);
}

int onStart(char *parameters, int length, char *error_message)
{
    return commandNotImplemented("chip:start", error_message);
}

int onScript(char *parameters, int length, char *error_message)
{
    return commandNotImplemented("chip:script", error_message);
}

int memWrite16(char *parameters, int length, char *error_message)
{
    return commandNotImplemented("chip:ram_w16", error_message);
}

int memRead16(char *parameters, int length, char *error_message)
{
    return commandNotImplemented("chip:ram_r16", error_message);
}

/////////////////////////////////////////////////////////////////

int onStop(char *parameters, int length, char *error_message)
{
    if (length > 0)
        strcpy (error_message, "Непустые параметры для команды \"chip:stop\" игнорируются");
    return CS_FINISH;
}

int regWrite(char *parameters, int param_len, char *error_message)
{
for (int i=0; i<=param_len; i++)
printf("%2d  ", parameters[i]);
   printf("\n param_len = %d\n", param_len);
    if (param_len < 8)
    {
	strcpy (error_message, "Недостаточна длина параметров для команды 'chip:reg_w'");
        return CS_ERROR;
    }
    long *p = (long*)parameters;
    char *data = parameters + 8;
    unsigned long addr = *p;
printf ("addr = %ld\n", addr);
addr*=4;
    unsigned long num_byte = *(p+1);
printf ("num_byte = %ld\n", num_byte);
num_byte*=4;
    if (num_byte > param_len-8)
    {
	sprintf (error_message, "Размер принятых данных меньше объявленного %ld", num_byte);
        return CS_ERROR;
    }

void *virtual_base;
long ps = sysconf(_SC_PAGESIZE);
off_t offset = 0xC0000000;
off_t pa_offset = offset & ~(ps-1);
size_t delta = offset - pa_offset;
size_t length = 0xC0000;
int fd = open("/dev/mem", (O_RDWR | O_SYNC));

    virtual_base = mmap(NULL, length + delta, (PROT_READ | PROT_WRITE), MAP_SHARED, fd, pa_offset);
    if(virtual_base == MAP_FAILED) {
        strcpy(error_message, "Ошибка mmap()");
        return CS_FATAL;
    }

    memcpy(virtual_base + addr + delta, (void*)data, num_byte);
return CS_NO_ERROR;
}

int regRead(char *parameters, int length, char *error_message)
{
    if (length < 8)
    {
	strcpy (error_message, "Недостаточная длина параметров для команды 'chip:reg_r'");
        return CS_ERROR;
    }
return CS_NO_ERROR;
}

int memWrite32(char *parameters, int param_len, char *error_message)
{
    if (param_len < 8)
    {
	strcpy (error_message, "Недостаточна длина параметров для команды 'chip:ram_w32'");
        return CS_ERROR;
    }
    long *p = (long*)parameters;
    char *data = parameters + 8;
    unsigned long addr = *p;
    unsigned long num_byte = (*(p+1)) * 4;
    if (num_byte > param_len-8)
    {
	sprintf (error_message, "Размер принятых данных меньше объявленного %ld", num_byte);
        return CS_ERROR;
    }
printf ("addr = %ld\n", addr);
addr*=4;

void *virtual_base;
long ps = sysconf(_SC_PAGESIZE);
off_t offset = 0xC0000000;
off_t pa_offset = offset & ~(ps-1);
size_t delta = offset - pa_offset;
size_t length = 0xC0000;
int fd = open("/dev/mem", (O_RDWR | O_SYNC));

    virtual_base = mmap(NULL, length + delta, (PROT_READ | PROT_WRITE), MAP_SHARED, fd, pa_offset);
    if(virtual_base == MAP_FAILED) {
        strcpy(error_message, "Ошибка mmap()");
        return CS_FATAL;
    }

    memcpy(virtual_base + addr + delta, (void*)data, num_byte);
return CS_NO_ERROR;
}

int memRead32(char *parameters, int length, char *error_message)
{
    if (length < 8)
    {
	strcpy (error_message, "Недостаточная длина параметров для команды 'chip:ram_r32'");
        return CS_ERROR;
    }
return CS_NO_ERROR;
}

//////////////////////////////////////////////////////////////////////////////////

int processChipCommand(char *command, int length, char *error_message)
{
    int key_len;
    if ((key_len = formatted(command, length, "load")) > 0)
       return onLoad(command+key_len, length-key_len, error_message);
    if (key_len = formatted(command, length, "start")) > 0)
        return onStart(command+key_len, length-key_len, error_message);
    if (key_len = formatted(command, length, "stop")) > 0)
        return onStop(command+key_len, length-key_len, error_message);
    if (key_len = formatted(command, length, "script")) > 0)
        return onScript(command+key_len, length-key_len, error_message);
    if (key_len = formatted(command, length, "ram_w16")) > 0)
        return memWrite16(command+key_len, length-key_len, error_message);
    if (key_len = formatted(command, length, "ram_r16")) > 0)
        return memRead16(command+key_len, length-key_len, error_message);
    if (key_len = formatted(command, length, "ram_w32")) > 0)
        return memWrite32(command+key_len, length-key_len, error_message);
    if (key_len = formatted(command, length, "ram_r32")) > 0)
        return memRead32(command+key_len, length-key_len, error_message);
    if (fkey_len = ormatted(command, length, "reg_w")) > 0)
        return regWrite(command+key_len, length-key_len, error_message);
    if (key_len = formatted(command, length, "reg_r")) > 0)
        return regRead(command+key_len, length-key_len, error_message);

    return commandUnknown(command, error_message);
}

