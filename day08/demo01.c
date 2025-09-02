#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>

#define HISTORY_SIZE 5

char history[HISTORY_SIZE][128];
int history_count = 0;

void addto_history(const char *cmd){
    if(history_count == HISTORY_SIZE){
        for(int i = 0; i < HISTORY_SIZE; i++){
            strcpy(history[i-1], history[i]);
        }
        strcpy(history[HISTORY_SIZE-1], cmd);
    }
    else{
        strcpy(history[history_count++], cmd);
    }

}

void print_history(){
    for(int i = 0; i < history_count; i++){
        printf("%d: %s\n", i+1, history[i]);
    }
}




int main() {
    // Your code here

    char input[128];
    char *command, *argument;

    while(1){
        printf("Enter FTP command: \n");
        fgets(input, sizeof(input), stdin);

        input[strcspn(input, "\n")] = '\0';

        if(strcasecmp(input, "HISTORY") == 0){
            print_history();
            continue;
        }
        if(strcasecmp(input, "QUIT") == 0){
            printf("Good bye DESD\n God bless you\n");

            break;
        }


        addto_history(input);

        command = strtok(input, " ");
        printf("command: %s\n", command);

        argument = strtok(NULL, " ");
        if(argument && argument[0] != '\0'){
            printf("argument: %s\n", argument);
        }
        else{
            printf("argument: (null)\n");

        }
        printf("\n");
    }
    return 0;
}