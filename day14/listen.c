#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>

#define PORT 8080
#define IP "127.0.0.1"

int main(void){
    int socket_fd;
    struct sockaddr_in server_addr;

    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(socket_fd < 0){
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }

    printf("Socket created successfully\n");
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(IP);
    server_addr.sin_port = htons(PORT);


    if(bind(socket_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0){
        perror("bind failed");
        close(socket_fd);
        exit(EXIT_FAILURE);
    }
    printf("Bind successful\n");

    //start listening for incoming connections
    if(listen(socket_fd, 5) < 0){
        perror("Listen failed");
        close(socket_fd);
        exit(EXIT_FAILURE);

    }

    printf("server is Listening on IP=%s, PORT=%d\n", IP, PORT);
    close(socket_fd);
    


    return 0;
}