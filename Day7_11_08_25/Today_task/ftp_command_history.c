#include<stdio.h>
#include<string.h>
#include<strings.h>

#define HISTORY_SIZE 5

char history[HISTORY_SIZE][128];
int history_count = 0;

void add_to_history(const char *cmd)
{
    if(history_count == HISTORY_SIZE)
    {
        for(int i=1; i<HISTORY_SIZE; i++)
        {
            strcpy(history[i-1],history[i]);
        }
        strcpy(history[HISTORY_SIZE-1],cmd);
     }
     else 
        strcpy(history[history_count++],cmd);

}

void print_history()
{
    printf("--Comment history--\n");
    for (int i = 0; i < history_count; i++)
    {
        printf("%d %s\n",i+1,history[i]);
    }
    
}

int main()
{
    char input[128];
    char *command, *argument;
    while(1)
    {
        printf("Enter FTP command : ");
        fgets(input,sizeof(input),stdin);
        // removes the trailing newline
        input[strcspn(input,"\n")] = '\0';

        if(strcasecmp(input,"HISTORY")==0)
        {
            print_history();
            continue;
        }
        if(strcasecmp(input,"QUIT")==0){
            printf("GoodBye..\n");
            break;
        } 
    
        add_to_history(input);

        command = strtok(input," ");
        printf("COMMAND : %s\n",command);
    
        argument = strtok(NULL," ");
        if(argument && argument[0] != '\0')
            printf("ARGUMENT : %s\n",argument);
        else
            printf("ARGUMENT : <none>\n");
    }
    return 0;  
}