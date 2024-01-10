#include "header_f.h"

void extract_external_commands(char **external_commands)
{
  //Variable Declaration
  char buffer[25] = {'\0'};
  int index = 0, ext_index = 0;
  char ch;
  //open the file using open system call 
  int file_des = open("external_commands.txt", O_RDONLY);  
  if(file_des == -1)
  {
      perror("OPEN COMMAND");
      exit(1);
  }
  //read all the commands from the file and store it in the 2D external_command array 
  while(read(file_des, &ch, 1) > 0)
  {
      if(ch != '\n')
      {
          buffer[index++] = ch;
      }
      else
      {
          buffer[index] = '\0';
          //allocate the memory for each command 
          external_commands[ext_index] = calloc(strlen(buffer), sizeof(char));
          strcpy(external_commands[ext_index++], buffer);
          //clear the buffer to hold next command 
          memset(buffer,'\0', 25);
          index = 0;
      }
  }
}

