#include <stdio.h>              // For input/output functions like printf, perror
#include <stdlib.h>             // For exit() and general utilities
#include <string.h>             // For memset(), strlen()
#include <unistd.h>             // For close(), write(), read()
#include <sys/socket.h>         // For socket(), bind(), listen(), accept(), recv()
#include <netinet/in.h>         // For sockaddr_in and IP-related constants
#include <arpa/inet.h>          // For inet_ntop() to convert IP to readable form

#define PORT    8080
#define IP      "127.0.0.1"

int main()
{
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size;
    char buffer[1024];
    char client_ip[INET_ADDRSTRLEN];

    server_fd = socket(AF_INET,SOCK_STREAM,0);
    if(server_fd < 0)
    {
        perror("socket creation failed!");
        exit(0);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr(IP);

    // This allows your server to reuse the port immediately after it's closed, avoiding the "Address already in use" error during development.
    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    if(bind(server_fd,(struct sockaddr*)&server_addr,sizeof(server_addr))<0){
        perror("Bind failed!\n");
        close(server_fd);
        exit(1);
    }

    listen(server_fd,5);
    printf("Server listning on Port 8080...\n");

    addr_size = sizeof(client_addr);
    client_fd = accept(server_fd,(struct sockaddr*)&client_addr,&addr_size);

    inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, sizeof(client_ip));
    printf("Connected to client: %s:%d\n", client_ip, ntohs(client_addr.sin_port));

    // Task 2:- Modify the server to send a welcome message to the client immediately after connection. ( Hint :- use write() and accept() )
    char *welcome_msg = "Welcome to the server";
    if(write(client_fd,welcome_msg,strlen(welcome_msg))<0){
        perror("Write to client failed!");
    }

    // Task 1:- Modify the server to print client messages on the server terminal.
    // Start receiving messages from client
    // Loop continues until client disconnects or error occurs
    while(1){
        memset(buffer, 0, sizeof(buffer));
        int byte_received = recv(client_fd, buffer, sizeof(buffer),0);
        if(byte_received <= 0){
            printf("Client disconnected or error occurred.\n");
            break;
        }
        // Task - Modify the server to terminate connection if client sends "exit".
        if(strncmp(buffer,"exit",4)==0){
            printf("Client requested to close the connection.\n");
            break;
        }
         printf("Client says: %s\n", buffer);
    }

    close(client_fd);
    close(server_fd);

    return 0;
}