///
/// Главный заголовочный файл для программы cs
///

#define _Debug_

#ifdef _Debug_
#define  CS_Perror_(mes) {perror(mes); printf("\n%s\n", mes);}
#define  CS_PrintError_(mes) printf("%s. Команда игнорируется.\n\n", mes)
#define  CS_Warning_(mes) printf("Предупреждение: %s.\n\n", mes)
#else
#define  CS_Perror_(mes) perror(mes)
#define  CS_PrintError_(mes)
#define  CS_Warning_(mes)
#endif

/// Коды ошибок
#define CS_NO_ERROR  0
#define CS_WARNING   1
#define CS_ERROR     2
#define CS_FATAL     3
#define CS_FINISH    4

int processFileCommand(char *command, int length, char *error_message);
int processChipCommand(char *command, int length, char *error_message);
int formatted(char *command, int command_len, char *key);
int commandNotImplemented(char *command, char *error_message);
int commandUnknown(char *command, char *error_message);

