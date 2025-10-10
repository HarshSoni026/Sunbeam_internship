#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<netinet/in.h>

#define PORT 8080

#define IP "127.0.0.1"

int main(void){

    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);

    if(socket_fd < 0){
        perror("SOCKET ERROR");
        exit(EXIT_FAILURE);
    }
    printf("Socket created successfully\n, socket_fd = %d\n", socket_fd);
    close(socket_fd);
    return 0;
}