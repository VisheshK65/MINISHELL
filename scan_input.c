#include "header_f.h"

int status;
pid_t child_extern;

void scan_input(char *prompt, char *input_string)
{
    while(1)
    {
        printf(ANSI_COLOR_RED"[%s]$"ANSI_COLOR_RESET, prompt);
        //Clear the ouput buffer
        fflush(stdout);
        //Read the input command from the user 
        scanf("%[^\n]", input_string);
        //Clear the input buffer 
        getchar();

        //To customise the input prompt. Whenever the  
        if(!strncmp("PS1=", input_string, 4))
        {
            //change the name of the MINISHELL  
            if(input_string[4] != ' ')
            {
                strcpy(prompt, &input_string[4]);
                memset(input_string, '\0', 25);
                continue;
            }
            //else print an error if PS1=<NAME> pattern is not been followed 
            printf(ANSI_COLOR_CYAN"TO change the  name of minishell follow : \"PS1=<ANY_NAME>\"\n"ANSI_COLOR_RESET);
        }
        else
        {
            //function to hold which command is been passed 
            char *command = get_command(input_string);
            //find which type of command is the given function 
            int command_type = check_command_type(command);

            //If the command provided by the user is an External Command then run the below block 
            if(command_type == EXTERNAL)
            {
                //Create a child process so that the child can run the given command and parent can wait for the process to get over 
                child_extern = fork();
                if(child_extern == 0)
                {
                    // Tokenize the command to separate the command and its arguments
                    char *args[100];
                    int arg_count = 0;

                    // Tokenize the command using space as the delimiter
                    char *token = strtok(input_string, " ");
                    while (token != NULL)
                    {
                        args[arg_count++] = token;
                        token = strtok(NULL, " ");
                    }

                    // Set the last element of the args array as NULL for execvp
                    args[arg_count] = NULL;

                    // Execute the external command using execvp
                    int ret_value = execvp(args[0], args);

                    // If execvp returns, it means the command execution failed
                    printf("Failed to execute command: %s\n", command);
                    exit(1);
                }
                else if(child_extern > 0)
                {
                    wait(&status);
                    if(WIFEXITED(status))
                    {
                        printf("\nChild process running the command with PID [%d] has terminated normally\n", child_extern);
                    } 
                }
                //If the fork system call doesn't work print the error message 
                else
                {
                    printf("\nFork system failed to execute\n");
                    exit(2);
                }
            }

            //Function to implement the echo command
            echo(input_string, status);

            //Function to execute the Builtin Commands 
            execute_internal_commands(input_string);
        }
    }
}
