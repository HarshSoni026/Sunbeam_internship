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

    // Reuse port immediately after close()
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
    printf("Server listening on Port 8080...\n");

    addr_size = sizeof(client_addr);
    client_fd = accept(server_fd,(struct sockaddr*)&client_addr,&addr_size);
    if (client_fd < 0) {
        perror("Accept failed");
        close(server_fd);
        exit(1);
    }

    inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, sizeof(client_ip));
    printf("Connected to client: %s:%d\n", client_ip, ntohs(client_addr.sin_port));

    // Send welcome message
    char *welcome_msg = "Welcome to the server\r\n";
    if(write(client_fd,welcome_msg,strlen(welcome_msg))<0){
        perror("Write to client failed!");
    }

    // Receive filename from client
    memset(buffer, 0, sizeof(buffer));
    int bytes = recv(client_fd, buffer, sizeof(buffer)-1, 0);
    if (bytes <= 0) {
        printf("No filename received. Closing connection.\n");
        close(client_fd);
        close(server_fd);
        return 0;
    }
    buffer[bytes] = '\0';

    
    buffer[strcspn(buffer, "\r\n")] = '\0';
    printf("Client requested file: %s\n", buffer);

    // Try to open requested file
    FILE *fp = fopen(buffer, "rb");
    if (fp == NULL) {
        // Task 1 – File not found handling
        char *error_msg = "550 File not found\r\n";
        send(client_fd, error_msg, strlen(error_msg), 0);
        printf("File not found: %s\n", buffer);
    } else {
        // Send file contents
        char file_buf[1024];
        int n;
        while ((n = fread(file_buf, 1, sizeof(file_buf), fp)) > 0) {
            if (send(client_fd, file_buf, n, 0) < 0) {
                perror("File send error");
                break;
            }
        }
        fclose(fp);
        printf("File transfer completed: %s\n", buffer);
    }

    close(client_fd);
    close(server_fd);

    return 0;
}
