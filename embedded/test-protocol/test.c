#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <unistd.h>
#include <arpa/inet.h>

#include "cs.h"
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>


/// Идентификационная метка команды 0xBEAD0618BEAD0001
//const long IdCode = 0xBEAD0618BEAD0001;
const char *Id = "NEBO:"; //"\0xBE\0xAD\0x06\0x18\0xBE\0xAD\0x00\0x01\0";
const int  IdLen = 5;  // длина идентификатора Id
const char Del = '?';

#define SIZE 0x200000
#define SIZE_S 0x100

const int quelen = 30;   // максимальная разрешенная длина очереди для accept(...)
#ifdef _Debug_
int maxnum = 2000;       // ограничение цикла приёма команд
#endif
//const int ERR = 5;     // Ошибки с номерами больше ERR считаются фатальными и влекут выход из программы,
                         // а ошибки с номерами <= ERR являются предупреждениями и ведут к игнорированию
                         // текущей команды и переходу к ожиданию следующей команды.

int  processCommand(char *command, int length, char *error_message);
int  processEchoCommand(char *command, int length, char *error_message);
bool checkIdError(char* id, char* buf, int len);


int main(int argc, char *argv[])
{
    int ret = 0;
    char buffer[SIZE];
    int client_fd;
#ifdef _Debug_
    int counter=0;
#endif

    printf("Программа cs: версия %s %s\n", __DATE__, __TIME__);
//  printf("Id = %s\n\n", Id);

    struct sockaddr_in my_addr;
    /// Запуск сервера
    int tcp_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (tcp_socket == -1)
	{
	   CS_Perror_("Ошибка создания сокета");
	   exit(EXIT_FAILURE);
	}

    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(3308);
    if (inet_aton("192.168.0.2", &my_addr.sin_addr) == 0)
	{
	   CS_Perror_("Ошибка функции inet_aton()");
	   exit(EXIT_FAILURE);
	}

    if (bind(tcp_socket, (struct sockaddr*) &my_addr, sizeof(my_addr)) != 0)
	{
	   CS_Perror_("Ошибка функции bind()");
	   exit(EXIT_FAILURE);
	}

    if (listen(tcp_socket, quelen) != 0)
	{
	   CS_Perror_("Ошибка функции listen()");
	   exit(EXIT_FAILURE);
	}

#ifdef _Debug_
    if (argc >= 2)
	maxnum = atoi(argv[1]);
#endif

while (1) {

	char error_message[SIZE_S];
        int n_read = 0, n_cur;
#ifdef _Debug_
        printf("Step %d of %d\n", counter, maxnum);
#endif
        /// Приём команды
	client_fd = accept(tcp_socket, NULL, NULL);
	printf("\naccept %d  client=%d\n", counter, client_fd);

	n_read = read(client_fd, buffer, sizeof(buffer)-1);

        printf("Принята команда: %d bytes\n", n_read);

	if ( n_read > 0)
	{
        *error_message = 0;
	    if (n_read >= sizeof(buffer)-2)
	    {
		    CS_Perror_("Длина команды превышает длину буфера");
		    close(client_fd);
		    ret = EXIT_FAILURE;
		    break;
	    }
	    buffer[n_read] = 'Q';

	    if (n_read < IdLen || checkIdError(Id, buffer, IdLen))
	    {
    		CS_PrintError_("Неверная идентификационная метка");
	    }
	    else
	    {
		    int error;
		    if (error = processCommand(buffer+IdLen, n_read-IdLen, error_message) != CS_NO_ERROR)
		    {
                if (error == CS_FINISH)
                {
                    if (*error_message)
                        CS_Warning_(error_message);
                    break;
                }
		        else if (error == CS_WARNING)
	    		    CS_Warning_(error_message);
		        else if (error == CS_ERROR)
		    	    CS_PrintError_(error_message);
		        else
		        {
			        char message[SIZE_S+20];
			        sprintf(message, "Ошибка: %s !!!\n\n", error_message);
                    CS_Perror_(message);
			        close(client_fd);
			        ret = EXIT_FAILURE;
			        break;
		        }
		    }
	    }
	}
	else
	{
	    sleep(1);
	}

	close(client_fd);
#ifdef _Debug_
	//if (counter++ >= maxnum);
	  //  break;
#endif
    }

    if (close(tcp_socket) != 0)
    {
       CS_Perror_("Ошибка закрытия сокета");
	exit(EXIT_FAILURE);
    }

    printf ("\nУспешное завершение программы\n");
    return 0;
}

bool checkIdError(char* id, char* buf, int len)
{
    for (int i=0; i<len; ++i)
        if (id[i] != buf[i])
            return false;

    return true;
}

int processCommand(char *command, int length, char *error_message)
{
    int ret = CS_ERROR;
    //char *parameters;
    *error_message = 0;

    if (length <= 5)
	    strcpy(error_message, "Недостаточная длина команды");
    if (strncmp(command, "echo", 4) == 0)
    	ret = processEchoCommand(command, length, error_message);
    else if (strncmp(command, "file:", 5) == 0)
	    ret = processFileCommand(command+5, length-5, error_message);
    else if (strncmp(command, "chip:", 5) == 0)
	    ret = processChipCommand(command+5, length-5, error_message);
    else 
	    strcpy(error_message, "Недопустимый префикс");

    return ret;
}

int processEchoCommand(char *command, int length, char *error_message)
// echo?.......
{
    if (command[4] != Del)
    {
	strcpy(error_message, "Отсутствует разделитель после \"echo\"");
	return CS_ERROR;
    }
    printf("%s\n", command+5);
    return CS_NO_ERROR;
 }

int formatted(char *command, int command_len, char *key)
{
    int len = strlen(key);
    if (command_len <= len || strncmp(command, key, len) != 0 || command[len] != Del)
        return -1;
    printf("%s\n", key);
    return len + 1;
}

int commandNotImplemented(char *command, char *error_message)
{
    sprintf(error_message, "Команда %s не реализована\n", command);
    return CS_ERROR;
}

int commandUnknown(char *command, char *error_message)
{
    int maxsize = SIZE_S - strlen("Неизвестная команда") - 4;
    if (strlen(command) > maxsize)
	command[maxsize] = 0;
    sprintf(error_message, "Неизвестная команда %s\n", command);
    return CS_ERROR;
}

