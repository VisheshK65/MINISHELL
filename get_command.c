#include "header_f.h"

char *safe_get_command(const char *input_string, char *command_buffer, size_t buffer_size) {
    if (!input_string || !command_buffer || buffer_size == 0) {
        return NULL;
    }
    
    size_t index = 0;
    const char *ptr = input_string;
    
    // Skip leading whitespace
    while (*ptr == ' ' || *ptr == '\t') {
        ptr++;
    }
    
    // Extract command until space, tab, or null terminator
    while (*ptr != '\0' && *ptr != ' ' && *ptr != '\t' && index < buffer_size - 1) {
        command_buffer[index++] = *ptr++;
    }
    
    // Null terminate
    command_buffer[index] = '\0';
    
    // Return NULL if command is empty or only whitespace
    if (index == 0) {
        return NULL;
    }
    
    return command_buffer;
}

// Memory management functions
int initialize_shell_context(shell_context_t *ctx) {
    if (!ctx) {
        return FAILURE;
    }
    
    // Initialize with default values
    strncpy(ctx->prompt, "COMMAND", sizeof(ctx->prompt) - 1);
    ctx->prompt[sizeof(ctx->prompt) - 1] = '\0';
    ctx->last_exit_status = 0;
    ctx->last_child_pid = 0;
    ctx->external_commands = NULL;
    ctx->external_cmd_count = 0;
    
    return SUCCESS;
}

void cleanup_shell_context(shell_context_t *ctx) {
    if (!ctx) {
        return;
    }
    
    // Free external commands array
    if (ctx->external_commands) {
        for (int i = 0; i < ctx->external_cmd_count; i++) {
            free(ctx->external_commands[i]);
        }
        free(ctx->external_commands);
        ctx->external_commands = NULL;
    }
    ctx->external_cmd_count = 0;
}
