#include "header_f.h"

int safe_cd_command(const char *path) {
    if (!path) {
        // No path provided, change to home directory
        const char *home = getenv("HOME");
        if (!home) {
            fprintf(stderr, "cd: HOME not set\n");
            return FAILURE;
        }
        path = home;
    }
    
    if (chdir(path) == -1) {
        perror("cd");
        return FAILURE;
    }
    
    return SUCCESS;
}

int execute_builtin_commands(const char *input_string, shell_context_t *ctx) {
    if (!input_string || !ctx) {
        return FAILURE;
    }
    
    // Handle exit command
    if (strncmp("exit", input_string, 4) == 0) {
        // Check if there's an exit code specified
        const char *exit_code_str = input_string + 4;
        while (*exit_code_str == ' ' || *exit_code_str == '\t') {
            exit_code_str++;
        }
        
        int exit_code = 0;
        if (*exit_code_str != '\0') {
            char *endptr;
            long code = strtol(exit_code_str, &endptr, 10);
            if (*endptr == '\0' && code >= 0 && code <= 255) {
                exit_code = (int)code;
            } else {
                printf(ANSI_COLOR_YELLOW "exit: numeric argument required\n" ANSI_COLOR_RESET);
                exit_code = 2;
            }
        }
        
        printf(ANSI_COLOR_GREEN "Goodbye!\n" ANSI_COLOR_RESET);
        cleanup_shell_context(ctx);
        exit(exit_code);
    }
    
    // Handle pwd command
    if (strncmp("pwd", input_string, 3) == 0 && 
        (input_string[3] == '\0' || input_string[3] == ' ')) {
        safe_pwd_command();
        return SUCCESS;
    }
    
    // Handle cd command
    if (strncmp("cd", input_string, 2) == 0) {
        const char *path = NULL;
        
        if (input_string[2] == '\0') {
            // cd with no arguments - go to home
            path = NULL;
        } else if (input_string[2] == ' ') {
            // cd with path
            path = input_string + 3;
            // Skip leading whitespace
            while (*path == ' ' || *path == '\t') {
                path++;
            }
            // If only whitespace after cd, treat as cd with no args
            if (*path == '\0') {
                path = NULL;
            }
        } else {
            // Invalid cd command format
            printf(ANSI_COLOR_YELLOW "cd: usage: cd [directory]\n" ANSI_COLOR_RESET);
            return FAILURE;
        }
        
        return safe_cd_command(path);
    }
    
    return FAILURE;
}


