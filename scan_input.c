#include "header_f.h"

// Global variable for signal handling
static shell_context_t *g_current_shell = NULL;

int safe_input_read(char *buffer, size_t buffer_size) {
    if (!buffer || buffer_size == 0) {
        return FAILURE;
    }
    
    if (!fgets(buffer, buffer_size, stdin)) {
        if (feof(stdin)) {
            return FAILURE; // EOF reached
        }
        if (ferror(stdin)) {
            perror("Input read error");
            return FAILURE;
        }
    }
    
    // Remove newline character if present
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
    }
    
    return SUCCESS;
}

void display_prompt(const char *prompt) {
    printf(ANSI_COLOR_RED "[%s]$ " ANSI_COLOR_RESET, prompt);
    fflush(stdout);
}

int parse_ps1_command(const char *input, char *new_prompt, size_t prompt_size) {
    if (!input || !new_prompt || prompt_size == 0) {
        return FAILURE;
    }
    
    if (strncmp("PS1=", input, 4) != 0) {
        return FAILURE; // Not a PS1 command
    }
    
    if (input[4] == '\0' || input[4] == ' ') {
        printf(ANSI_COLOR_CYAN "To change the name of minishell follow: \"PS1=<ANY_NAME>\"\n" ANSI_COLOR_RESET);
        return FAILURE;
    }
    
    // Safely copy the new prompt
    strncpy(new_prompt, &input[4], prompt_size - 1);
    new_prompt[prompt_size - 1] = '\0';
    
    return SUCCESS;
}

int shell_main_loop(shell_context_t *ctx) {
    if (!ctx) {
        fprintf(stderr, "Invalid shell context\n");
        return FAILURE;
    }
    
    g_current_shell = ctx; // For signal handling
    char input_buffer[MAX_INPUT_SIZE];
    char command_buffer[MAX_COMMAND_SIZE];
    char new_prompt[MAX_PROMPT_SIZE];
    
    while (1) {
        display_prompt(ctx->prompt);
        
        // Safely read input
        if (safe_input_read(input_buffer, sizeof(input_buffer)) != SUCCESS) {
            if (feof(stdin)) {
                printf("\n" ANSI_COLOR_GREEN "Goodbye!\n" ANSI_COLOR_RESET);
                break;
            }
            continue;
        }
        
        // Skip empty input
        if (strlen(input_buffer) == 0) {
            continue;
        }
        
        // Handle PS1 prompt change
        if (parse_ps1_command(input_buffer, new_prompt, sizeof(new_prompt)) == SUCCESS) {
            strncpy(ctx->prompt, new_prompt, sizeof(ctx->prompt) - 1);
            ctx->prompt[sizeof(ctx->prompt) - 1] = '\0';
            continue;
        }
        
        // Extract command name safely
        char *command = safe_get_command(input_buffer, command_buffer, sizeof(command_buffer));
        if (!command) {
            continue;
        }
        
        // Determine command type
        int command_type = check_command_type(command, ctx);
        
        // Execute based on command type
        switch (command_type) {
            case EXTERNAL:
                ctx->last_exit_status = execute_external_command(input_buffer, ctx);
                break;
            case BUILTIN:
                // Handle builtin commands (including echo)
                if (handle_echo_commands(input_buffer, ctx) != SUCCESS) {
                    ctx->last_exit_status = execute_builtin_commands(input_buffer, ctx);
                }
                break;
            case NO_COMMAND:
                printf(ANSI_COLOR_YELLOW "Command not found: %s\n" ANSI_COLOR_RESET, command);
                ctx->last_exit_status = 127; // Standard "command not found" exit code
                break;
            default:
                printf(ANSI_COLOR_RED "Unknown command type\n" ANSI_COLOR_RESET);
                ctx->last_exit_status = 1;
                break;
        }
    }
    
    return SUCCESS;
}

void signal_handler(int sig_num) {
    switch (sig_num) {
        case SIGINT:
            printf("\n" ANSI_COLOR_YELLOW "Received SIGINT (Ctrl+C)\n" ANSI_COLOR_RESET);
            if (g_current_shell) {
                display_prompt(g_current_shell->prompt);
            }
            break;
        case SIGTERM:
            printf("\n" ANSI_COLOR_YELLOW "Received SIGTERM, shutting down gracefully...\n" ANSI_COLOR_RESET);
            if (g_current_shell) {
                cleanup_shell_context(g_current_shell);
            }
            exit(0);
            break;
        default:
            printf("\n" ANSI_COLOR_RED "Received unknown signal: %d\n" ANSI_COLOR_RESET, sig_num);
            break;
    }
}
