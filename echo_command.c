#include "header_f.h"

void safe_pwd_command(void) {
    char cwd[MAX_PATH_SIZE];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("%s\n", cwd);
    } else {
        perror("pwd");
    }
}

int handle_echo_commands(const char *input_string, shell_context_t *ctx) {
    if (!input_string || !ctx) {
        return FAILURE;
    }
    
    // Handle echo $? - exit status of previous command
    if (strncmp("echo $?", input_string, 7) == 0) {
        printf("%d\n", ctx->last_exit_status);
        return SUCCESS;
    }
    
    // Handle echo $$ - PID of current shell process
    if (strncmp("echo $$", input_string, 7) == 0) {
        printf("%d\n", getpid());
        return SUCCESS;
    }
    
    // Handle echo $SHELL - current working directory (safer than system call)
    if (strncmp("echo $SHELL", input_string, 11) == 0) {
        safe_pwd_command();
        return SUCCESS;
    }
    
    // Handle regular echo command
    if (strncmp("echo ", input_string, 5) == 0) {
        const char *message = input_string + 5;
        // Skip leading whitespace
        while (*message == ' ' || *message == '\t') {
            message++;
        }
        printf("%s\n", message);
        return SUCCESS;
    }
    
    // Not an echo command
    return FAILURE;
}

