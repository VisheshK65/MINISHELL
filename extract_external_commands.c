#include "header_f.h"

int load_external_commands(shell_context_t *ctx) {
    if (!ctx) {
        return FAILURE;
    }
    
    FILE *file = fopen("external_commands.txt", "r");
    if (!file) {
        perror("Could not open external_commands.txt");
        return FAILURE;
    }
    
    // First pass: count the number of commands
    int count = 0;
    char buffer[MAX_COMMAND_SIZE];
    
    while (fgets(buffer, sizeof(buffer), file)) {
        // Remove newline if present
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
            len--;
        }
        
        // Skip empty lines
        if (len > 0) {
            count++;
        }
    }
    
    if (count == 0) {
        fclose(file);
        return SUCCESS; // File is empty, but not an error
    }
    
    // Allocate memory for command pointers
    ctx->external_commands = calloc(count, sizeof(char*));
    if (!ctx->external_commands) {
        perror("Memory allocation failed");
        fclose(file);
        return FAILURE;
    }
    
    // Reset file pointer to beginning
    if (fseek(file, 0, SEEK_SET) != 0) {
        perror("fseek failed");
        free(ctx->external_commands);
        ctx->external_commands = NULL;
        fclose(file);
        return FAILURE;
    }
    
    // Second pass: read and store commands
    int index = 0;
    while (fgets(buffer, sizeof(buffer), file) && index < count) {
        // Remove newline if present
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
            len--;
        }
        
        // Skip empty lines
        if (len == 0) {
            continue;
        }
        
        // Allocate memory for the command string (+1 for null terminator)
        ctx->external_commands[index] = malloc(len + 1);
        if (!ctx->external_commands[index]) {
            perror("Memory allocation failed");
            // Cleanup already allocated memory
            for (int i = 0; i < index; i++) {
                free(ctx->external_commands[i]);
            }
            free(ctx->external_commands);
            ctx->external_commands = NULL;
            fclose(file);
            return FAILURE;
        }
        
        strcpy(ctx->external_commands[index], buffer);
        index++;
    }
    
    ctx->external_cmd_count = index;
    fclose(file);
    
    printf(ANSI_COLOR_GREEN "Loaded %d external commands\n" ANSI_COLOR_RESET, ctx->external_cmd_count);
    return SUCCESS;
}

int execute_external_command(const char *input_string, shell_context_t *ctx) {
    if (!input_string || !ctx) {
        return FAILURE;
    }
    
    // Create a copy of input for tokenization
    char input_copy[MAX_INPUT_SIZE];
    strncpy(input_copy, input_string, sizeof(input_copy) - 1);
    input_copy[sizeof(input_copy) - 1] = '\0';
    
    // Tokenize the command to separate command and arguments
    char *args[MAX_ARGS];
    int arg_count = 0;
    
    char *token = strtok(input_copy, " \t");
    while (token != NULL && arg_count < MAX_ARGS - 1) {
        args[arg_count++] = token;
        token = strtok(NULL, " \t");
    }
    args[arg_count] = NULL; // execvp requires NULL-terminated array
    
    if (arg_count == 0) {
        return FAILURE;
    }
    
    // Create child process
    pid_t child_pid = fork();
    
    if (child_pid == 0) {
        // Child process
        execvp(args[0], args);
        
        // If execvp returns, it failed
        fprintf(stderr, "Failed to execute command: %s\n", args[0]);
        exit(127); // Standard exit code for "command not found"
    } else if (child_pid > 0) {
        // Parent process
        ctx->last_child_pid = child_pid;
        int status;
        
        if (waitpid(child_pid, &status, 0) == -1) {
            perror("waitpid failed");
            return FAILURE;
        }
        
        if (WIFEXITED(status)) {
            int exit_code = WEXITSTATUS(status);
            printf(ANSI_COLOR_BLUE "Child process [%d] exited with status %d\n" ANSI_COLOR_RESET, 
                   child_pid, exit_code);
            return exit_code;
        } else if (WIFSIGNALED(status)) {
            int signal_num = WTERMSIG(status);
            printf(ANSI_COLOR_YELLOW "Child process [%d] terminated by signal %d\n" ANSI_COLOR_RESET, 
                   child_pid, signal_num);
            return 128 + signal_num; // Standard convention for signal termination
        }
    } else {
        // Fork failed
        perror("Fork failed");
        return FAILURE;
    }
    
    return SUCCESS;
}

