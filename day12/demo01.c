#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/types.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netinet/ip.h>

#define IP "127.0.0.1"
#define PORT 8081

int main(){
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size;

    char buffer[1024];
    char client_ip[INET_ADDRSTRLEN];

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(server_fd < 0){
        perror("server socket failed!");
        exit(EXIT_FAILURE);
    }
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr(IP);

    //reuse port immidietely after program terminaiton.
    int opt = 1;
    if(setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0){
        perror("setsockopt failed!");
        exit(EXIT_FAILURE); 
    }

    if(bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0){
        perror("bind failed!");
        exit(EXIT_FAILURE);
    }

    listen(server_fd, 5);
    printf("Server listening to port : %d", PORT);

    addr_size = sizeof(client_addr);
    client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &addr_size);
    if(client_fd < 0){
        perror("Client accept failed!");
        close(server_fd);
        exit(1);
    }

    inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, sizeof(client_ip));
    printf("connected to client : %s , %d\n", client_ip, ntohs(client_addr.sin_port));

    //send welcome message.
    char *welcome_msg = "Welcome to the server \r\n";
    if(write(client_fd, welcome_msg, strlen(welcome_msg))< 0){
        perror("failed to send welcome msg");

    }

    //receive filename from client
    memset(buffer, 0, sizeof(buffer));
    int bytes = recv(client_fd, buffer, sizeof(buffer) - 1, 0);
    if(bytes <= 0){
        printf("No filename received from client , closing connection.\n");
        close(client_fd);
        close(server_fd);
        return 0;
    }
    buffer[bytes] = '\0'; //null terminate the string
    buffer[strcspn(buffer, "\r\n")] = 0; //remove newline characters
    printf("Client requested file : %s\n", buffer);

    //try to open the file

    FILE *fp = fopen(buffer, "rb");
    if(fp == NULL){
        //file not found, send error message to client
        perror("File not found");
        char *error_msg = "550 file not found\r\n";
        send(client_fd, error_msg, strlen(error_msg), 0);
        
    }
    else{
        //send file content to client
        char file_buffer[1024];
        int n;
        while((n = fread(file_buffer, 1, sizeof(file_buffer), fp)) > 0){
            if(send(client_fd, file_buffer, n, 0) < 0){
                perror("failed to send file data to client");
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