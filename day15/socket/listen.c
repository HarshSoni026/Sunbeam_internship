#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<netinet/in.h>

int main(void){
    int socket_fd;
    struct sockaddr_in server_addr;

    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(socket_fd < 0){
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(8080);
    memset(&(server_addr.sin_zero), '\0', 8);

    if(bind(socket_fd, (struct sockaddr*)&server_addr, sizeof(server_addr))<0){
        perror("Bind failed");
        close(socket_fd);
        exit(EXIT_FAILURE);
    }
    printf("Bind to port 8080 successful\n");

    if(listen(socket_fd, 5) < 0){
        perror("Listen failed");
        close(socket_fd);
        exit(EXIT_FAILURE);
    }
    printf("Listening on port 8080...\n");
    close(socket_fd);
    return 0;
}