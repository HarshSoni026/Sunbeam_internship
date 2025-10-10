#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>

int main (){
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(socket_fd == -1){
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }
    printf("Socket created successfully with fd: %d\n", socket_fd);
    close(socket_fd);
    
    return 0;
}