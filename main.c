/*
NAME : VISHESH K
DATE : 23-05-2023
PROJECT : MINISHELL (IMPROVED VERSION)
IMPROVEMENTS: Security fixes, memory management, error handling
*/

#include "header_f.h"

static shell_context_t g_shell_ctx;

void cleanup_and_exit(int exit_code) {
    cleanup_shell_context(&g_shell_ctx);
    exit(exit_code);
}

void setup_signal_handlers(void) {
    signal(SIGINT, signal_handler);
    signal(SIGTERM, signal_handler);
}

int main(void)
{
    // Clear the screen safely
    printf("\033[2J\033[H");
    fflush(stdout);
    
    // Initialize shell context
    if (initialize_shell_context(&g_shell_ctx) != SUCCESS) {
        fprintf(stderr, "Failed to initialize shell context\n");
        return FAILURE;
    }
    
    // Setup signal handlers
    setup_signal_handlers();
    
    // Load external commands
    if (load_external_commands(&g_shell_ctx) != SUCCESS) {
        fprintf(stderr, ANSI_COLOR_YELLOW "Warning: Could not load external commands\n" ANSI_COLOR_RESET);
    }
    
    printf(ANSI_COLOR_GREEN "MINISHELL v2.0 - Enhanced Security Version\n" ANSI_COLOR_RESET);
    printf(ANSI_COLOR_CYAN "Type 'exit' to quit, 'PS1=<name>' to change prompt\n" ANSI_COLOR_RESET);
    
    // Main shell loop
    int result = shell_main_loop(&g_shell_ctx);
    
    // Cleanup and exit
    cleanup_and_exit(result);
    
    return SUCCESS;
}
