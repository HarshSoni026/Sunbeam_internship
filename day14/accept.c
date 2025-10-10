#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>

#define PORT 8080
#define IP "127.0.0.1"


int main(){
    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);

    if(socket_fd < 0){
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }
    struct sockaddr_in server_addr, client_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr(IP);

    memset(&(server_addr.sin_zero), 0, 8);

    printf("Socket created\n");
    if(bind(socket_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0){
        perror("Bind failed");
        close(socket_fd);
        exit(EXIT_FAILURE);

    }

    printf("Bind successfully.\n");

    if(listen(socket_fd, 5) < 0){
        perror("listen failed");
        close(socket_fd);
        exit(EXIT_FAILURE);

    }

    printf("server listening to IP=%s, PORT=%d\n", IP, PORT);
    
    //4. accept client connection.
    socklen_t addr_size;
    char client_ip[INET_ADDRSTRLEN];
    addr_size = sizeof(client_addr);

    int client_fd = accept(socket_fd, (struct sockaddr*)&client_addr, &addr_size);

    if(client_fd<0){
        perror("accept failed\n");
        close(socket_fd);
        exit(EXIT_FAILURE);
    }

    //display IP and port of connected client
    //inet_ntop() converts binary IP into string format.
    inet_ntop(AF_INET, &(client_addr.sin_addr), client_ip, INET_ADDRSTRLEN);
    printf("Client connected from %s:%d\n", client_ip, ntohs(client_addr.sin_port));

    //close both client and server socket.
    close(client_fd);
    close(socket_fd);
    return 0;
}