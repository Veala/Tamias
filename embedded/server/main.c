#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/select.h>
#include <sys/time.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

#define _Debug_

#define SIZE 0x200000
#define SIZE_S 0x100
const int quelen = 3;

fd_set rfds;
//timeval tv;

#ifdef _Debug_
#define  _Perror_(x) {perror(x); printf("\n%s\n", x);}
#else
#define  _Perror_(x) perror(x)
#endif


/*int readData(int &n, ssize_t &r, int& size, char (&refArray)[SIZE], timeval* tv) {
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

int readAllData(int &n, int& size, char (&refArray)[SIZE], timeval* tv) {
    ssize_t r=0;
    while (1) {
        int err = readData(n, r, size, refArray, tv);
        if (err != NO_ERROR) return err;
        n+=r;
        if (n>=size) return NO_ERROR;
    }
}
*/
int main(int argc, char *argv[])
{
char buffer[SIZE_S] = "Buffer 123456789 BBBBBBBBBBBBBBBBBBBBBBB\n";

    int tcp_socket = socket(AF_INET, SOCK_STREAM, 0);
    int client_fd, counter=0, n;
    if (tcp_socket == -1)
	{
	   _Perror_("socket creation error");
	   exit(EXIT_FAILURE);
	}

    struct sockaddr_in my_addr, client_addr;
    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(3308);
    if (inet_aton("192.168.0.2", &my_addr.sin_addr) == 0)
	{
	   _Perror_("inet_aton() failed");
	   exit(EXIT_FAILURE);
	}

//    if (bind(tcp_socket, (struct sockaddr*) &my_addr, sizeof(my_addr)) != 0)
    if (bind(tcp_socket, (struct sockaddr*) &my_addr, sizeof(my_addr)) != 0)
	{
	   _Perror_("bind() failed");
	   exit(EXIT_FAILURE);
	}

    if (listen(tcp_socket, quelen) != 0)
	{
	   _Perror_("bind");
	   exit(EXIT_FAILURE);
	}

    while (1) {
	client_fd = accept(tcp_socket, (struct sockaddr*)NULL, NULL);
	printf("\naccept %d  client=%d\n", counter, client_fd);
	printf("Buffer size = %d\n", sizeof(buffer));
sleep(1);
//n = recv(client_fd, buffer, sizeof(buffer)-1, 0);
n = read(client_fd, buffer, sizeof(buffer)-1);
	if ( n > 0)
	{
		printf("\nData received: %d bytes\n", n);
for (int i=0; i<n; i++)
printf("%c", buffer[i]);
printf("\n\n\n");
	}
        else
	{
		printf("\nNo data received from client. Answer = %d\n", n);
	}
sleep(1);
printf("%s\n", __TIME__);
for (int i=0; i<sizeof(buffer)-1; i++)
printf("%c", buffer[i]);
	/*if(fputs(buffer, stdout) == EOF)
		{
			printf("\n Error : Fputs error\n");
		}*/
printf ("\nEnd\n");
	close(client_fd);
	sleep(1);
	counter++;
	break;
    }

    if (close(tcp_socket) != 0)
{
       _Perror_("close tcp socket");
	   exit(EXIT_FAILURE);
	}

    return 0;
}

