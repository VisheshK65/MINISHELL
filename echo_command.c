#include "header_f.h"

void echo(char *input_string, int status)
{
   //command to return the exit status of the previous command 
   if(strncmp("echo $?", input_string, 7) == 0)
   {
       printf("%d\n", status);
   }
   //command to get the PID of the current shell process 
   if(strncmp("echo $$", input_string, 7) == 0)
   {
       printf("%d\n", getpid());
   }
   //command to get the path of the current shell in execution
   if(strncmp("echo $SHELL", input_string, 11) == 0)
   {
       system("pwd");
   }
}

