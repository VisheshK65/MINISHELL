#include "header_f.h"

char *get_command(char *input_string)
{
    static char command[25];
    int index = 0;
    while(1)
    {
        // read the characters of the command till we reach \n or ' '
        if(*input_string == ' ' || *input_string == '\0' )
            break;
        //store the character in the command array 
        command[index++] = *input_string;
        input_string++;
    }
    command[index] = '\0';
    return command;
}
