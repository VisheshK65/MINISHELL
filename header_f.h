#ifndef HEADER_F_H
#define HEADER_F_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdio_ext.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <errno.h>

// Buffer size constants
#define MAX_INPUT_SIZE      1024
#define MAX_COMMAND_SIZE    256
#define MAX_PROMPT_SIZE     64
#define MAX_ARGS            100
#define MAX_EXTERNAL_CMDS   200
#define MAX_PATH_SIZE       512

// Command types
#define BUILTIN		1
#define EXTERNAL	2
#define NO_COMMAND  3

// ANSI Color codes
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

// Return codes
#define FAILURE     -1
#define SUCCESS      0

// Shell context structure
typedef struct {
    char prompt[MAX_PROMPT_SIZE];
    int last_exit_status;
    pid_t last_child_pid;
    char **external_commands;
    int external_cmd_count;
} shell_context_t;

// Function declarations
int shell_main_loop(shell_context_t *ctx);
int safe_input_read(char *buffer, size_t buffer_size);
char *safe_get_command(const char *input_string, char *command_buffer, size_t buffer_size);
int check_command_type(const char *command, shell_context_t *ctx);
int handle_echo_commands(const char *input_string, shell_context_t *ctx);
int execute_builtin_commands(const char *input_string, shell_context_t *ctx);
int execute_external_command(const char *input_string, shell_context_t *ctx);
void signal_handler(int sig_num);
int load_external_commands(shell_context_t *ctx);
void cleanup_shell_context(shell_context_t *ctx);
int initialize_shell_context(shell_context_t *ctx);
void safe_pwd_command(void);
int safe_cd_command(const char *path);
void display_prompt(const char *prompt);
int parse_ps1_command(const char *input, char *new_prompt, size_t prompt_size);

#endif
