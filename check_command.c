#include "header_f.h"

int check_command_type(char *command)
{
    //2D array used to hold all the builtin commands 
    char *builtin_command[] = {"echo", "printf", "read", "cd", "pwd", "pushd", "popd", "dirs", "let", "eval", "set", "unset", "export", "declare", "typeset", "readonly", "getopts", "source", "exit", "exec", "shopt", "caller", "true", "type", "hash", "bind", "help", NULL};
   
    if(strcmp(command,"\0") == 0)
    {
        return NO_COMMAND;
    }

    //check if the given command is builtin command
    for(int index = 0; builtin_command[index] != NULL; index++)
    {
        if(strcmp(command, builtin_command[index]) == 0)
        {
            return BUILTIN;
        }
    }

    //array to hold the external commands read from a file 
    char *external_command[155] = {NULL};
    extract_external_commands(external_command);
    //check if the entered command is an external command 
    for(int ext_index = 0; external_command[ext_index] != NULL; ext_index++)
    {
        if(strcmp(command, external_command[ext_index]) == 0)
        {
            return EXTERNAL;
        }
    }
}






