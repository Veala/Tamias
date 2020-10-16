#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <unistd.h>
#include <arpa/inet.h>

//#define _Debug_

#define SIZE 0x200000
#define SIZE_S 0x100
const int quelen = 3;

#ifdef _Debug_
#define  _Perror_(x) {perror(x); printf("\n%s\n", x);}
#else
#define  _Perror_(x) perror(x)
#endif

int main(int argc, char *argv[])
{
    char buffer[SIZE];
    int client_fd, counter=0, n_read;
    int tcp_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (tcp_socket == -1)
	{
	   _Perror_("socket creation error");
	   exit(EXIT_FAILURE);
	}

    struct sockaddr_in my_addr;
    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(3308);
    if (inet_aton("192.168.0.2", &my_addr.sin_addr) == 0)
	{
	   _Perror_("inet_aton() failed");
	   exit(EXIT_FAILURE);
	}

    if (bind(tcp_socket, (struct sockaddr*) &my_addr, sizeof(my_addr)) != 0)
	{
	   _Perror_("bind() failed");
	   exit(EXIT_FAILURE);
	}

    if (listen(tcp_socket, quelen) != 0)
	{
	   _Perror_("listen() failed");
	   exit(EXIT_FAILURE);
	}

    while (1) {
	client_fd = accept(tcp_socket, (struct sockaddr*)NULL, NULL);
        printf("%s\n", __TIME__);
	printf("\naccept %d  client=%d\n", counter, client_fd);
	printf("Buffer size = %d\n", sizeof(buffer));
        n_read = read(client_fd, buffer, sizeof(buffer)-1);
	if ( n_read > 0)
	{
	    printf("Data received: %d bytes\n", n_read);
	    for (int i=0; i<n_read; i++)
		printf("%c", buffer[i]);
	}
        else
	{
	    printf("No data received from client. Answer = %d\n", n_read);
	}
	close(client_fd);
	sleep(1);
	if (counter++ >= 100);
	    break;
    }

    if (close(tcp_socket) != 0)
	{
       _Perror_("close tcp socket");
	   exit(EXIT_FAILURE);
	}

    return 0;
}

