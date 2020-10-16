
/*#include <stdlib.h>*/
#include <stdio.h>
#include <string.h>
#include "cs.h"

int processFileCommand(char *command, int length, char *error_message)
{

//    command[length] = 0;
    if (formatted(command, length, "receive") > 0)
       return commandNotImplemented("file:receive", error_message);
    if (formatted(command, length, "transmite") > 0)
       commandNotImplemented("file:transmite", error_message);

    return commandUnknown(command, error_message);
}

