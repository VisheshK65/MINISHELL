#include "header_f.h"

int check_command_type(const char *command, shell_context_t *ctx) {
    if (!command || !ctx) {
        return NO_COMMAND;
    }
    
    // Check for empty command
    if (strlen(command) == 0) {
        return NO_COMMAND;
    }
    
    // Builtin commands list - only implement what we actually support
    const char *builtin_commands[] = {
        "echo", "cd", "pwd", "exit", NULL
    };
    
    // Check if the given command is a builtin command
    for (int index = 0; builtin_commands[index] != NULL; index++) {
        if (strcmp(command, builtin_commands[index]) == 0) {
            return BUILTIN;
        }
    }
    
    // Check if the command is in our external commands list
    if (ctx->external_commands) {
        for (int ext_index = 0; ext_index < ctx->external_cmd_count; ext_index++) {
            if (ctx->external_commands[ext_index] && 
                strcmp(command, ctx->external_commands[ext_index]) == 0) {
                return EXTERNAL;
            }
        }
    }
    
    return NO_COMMAND;
}






