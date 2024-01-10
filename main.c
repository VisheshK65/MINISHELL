/*
NAME : VISHESH K
DATE : 23-05-2023
PROJECT : MINISHELL
*/

#include "header_f.h"

int main()
{
    //Clear the screen 
    system("clear");
    //Store the input 
    char input_str[25];
    //Store the prompt 
    char prompt[25] = "COMMAND";
    //function to scan the input passed through CLA : it can be either to change the Minishell name or it can be any command been passed 
    scan_input(prompt, input_str);

    return 0;
}
