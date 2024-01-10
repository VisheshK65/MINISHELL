#include "header_f.h"

void execute_internal_commands(char *input_string)
{
    //Execute commands like cd, pwd, exit

    if(strncmp("exit", input_string, 4) == 0)
    {
        exit(0);
    }

    if(strncmp("pwd", input_string, 3) == 0)
    {
        if(system("pwd") == -1)
        {
            perror("pwd");
            exit(1);
        }
    }

    if(strncmp("cd", input_string, 2) == 0)
    {
        //Take the input from the path provided, i.e avoid taking "cd " in the path 
        char *token = strtok(&input_string[3]," ");
        chdir(token);
    }
}


