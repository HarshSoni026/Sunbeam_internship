#include<stdio.h>
#include<string.h>

void handle_command(const char *cmd){
    if(strncmp(cmd, "USER", 4) == 0){
        printf("User command received\n");

    }
    else if(strncmp(cmd, "PASS", 4) == 0){
        printf("Pass command received\n");
    }
    else if(strncmp(cmd, "PWD", 3) == 0){
        printf("current directory\n");
    }
    else if(strncmp(cmd, "QUIT", 4) == 0){
        printf("Good Bye\n");

    }
    else{
        printf("Unknown Command\n");

    }
}


int main(void){

    char input[128];
    printf("Enter FTP command: ");
    fgets(input, sizeof(input), stdin);
    handle_command(input);
    


    return 0;
}