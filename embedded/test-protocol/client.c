#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define _Debug_

#ifdef _Debug_
#define  _Perror_(x) {perror(x); printf("\n%s\n", x);}
#else
#define  _Perror_(x) perror(x)
#endif

int main(int argc, char *argv[])
{
    char *message = "Hello server!\n22\n";
    char default_ip[] = "192.168.0.1";
    char* server_ip = default_ip;
    int serv_fd, counter=0;
    int cur_socket = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serv_addr;

    printf("%s client program\n", __TIME__);
    if (argc >= 2)
	server_ip = argv[1];
    printf("Default IP = %s\nServer IP = %s\n\n", default_ip, server_ip);

    if (cur_socket == -1)
	{
	   _Perror_("socket creation error");
	   exit(EXIT_FAILURE);
	}

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(3307);
    if (inet_aton(server_ip, &serv_addr.sin_addr) == 0)
	{
	   _Perror_("inet_aton for server failed");
	   exit(EXIT_FAILURE);
	}

    if (connect(cur_socket, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) != 0)
	{
	   _Perror_("connection failed");
	   exit(EXIT_FAILURE);
	}
    write(cur_socket, message, strlen(message));

    if (close(cur_socket) != 0)
{
       _Perror_("close tcp socket");
	   exit(EXIT_FAILURE);
	}

    return 0;
}

