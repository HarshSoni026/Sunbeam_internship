#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/socket.h>
#include<arpa/inet.h>

int main(int argc, char *argv[]) {
    FILE *fp = fopen("conf.txt", "r");
    if(fp == NULL){
        perror("failed to open file\n");
        return 1;
    }

    //client calls

    int n;
    int client_fd = socket(AF_INET, SOCK_STREAM, 0);

    char buf[1024];
    while((n = fread(buf, 1, sizeof(buf), fp))>0){
        //send to server
        send(client_fd, buf, n, 0);

    }

    fclose(fp);
    close(client_fd);
    


    return 0;
}