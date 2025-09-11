// Extract and print only the command arguments (parameters).

//Input: USER admin → Output: Command=USER, Argument=admin.


#include <stdio.h>
#include <string.h>

int main() {
    char input[128];
    char command[32],argument[96]="";
    printf("Enter FTP command: ");
    fgets(input, sizeof(input), stdin);

    //strcspn meaning in c This line removes the trailing newline from a string read by fgets.
    input[strcspn(input, "\n")] = 0; 

    //char *command, *argument;
    //command = strtok(input," ");
    //  argument = strtok(NULL, ""); // Null here returns the remaining input string after delimiter if present in the input string.
    //sscanf breaks the input string separated by the delimiter (here space is delimiter)
    if(sscanf( input, "%s%[^\n]" , command, argument)==2)
    
    printf("COMMAND : %s\n",command);
    if(argument && argument[0] != '\0')
        printf("ARGUMENT : %s\n",argument);
    else 
        printf("ARGUMENT = <none>\n");

    return 0;
}









