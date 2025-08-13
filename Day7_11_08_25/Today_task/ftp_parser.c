
#include <stdio.h>
#include <string.h>

int main() {
    char input[128];
    printf("Enter FTP command: ");
    fgets(input, sizeof(input), stdin);
    input[strcspn(input,"\n")];

    //strcspn meaning in c This line removes the trailing newline from a string read by fgets.
    input[strcspn(input, "\n")] = 0; 

    char *command, *argument;
    command = strtok(input," ");

    argument = strtok(NULL, "");

    printf("COMMAND : %s\n",command);
    if(argument && argument[0] != '\0')
        printf("ARGUMENT : %s\n",argument);
    else 
        printf("ARGUMENT = <none>\n");

    return 0;
}
