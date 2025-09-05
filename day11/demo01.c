#include<stdio.h>   //for input output functions like printf, scanf
#include<string.h>  // for memset(), strlen(), strcmp() etc.
#include<stdlib.h>  // for exit() and other general functions
#include<stdbool.h> // for boolean data type
#include<sys/socket.h> // for socket(), bind(), listen(), accept() and other socket functions
#include<arpa/inet.h> // for inet_addr and other internet address functions and inet_ntop to IP to readable format
#include<unistd.h>  // for close(), read(), write() and other POSIX functions
#include<netinet/in.h>  // for sockaddr_in and other internet address family structures
#include<netinet/ip.h> // for iphdr structure

#define PORT 8081
#define IP "127.0.0.1"


int main(){
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size;

    char buffer[1024] = {0};
    char client_ip[INET_ADDRSTRLEN];

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(server_fd < 0){
        perror("Socket creation failed");
        exit(EXIT_FAILURE); 
    }
    printf("Socket created successfully\n");
    
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr(IP);

    //reuse port immidietly after close
    int opt = 1;
    if(setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0){
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    if(bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0){
        perror("Bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Binded successfully\n");
    listen(server_fd, 5);
    printf("Listening on port %d\n", PORT);

    addr_size = sizeof(client_addr);
    client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &addr_size);
    if(client_fd < 0){
        perror("Accept failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }
    inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, sizeof(client_ip));
    printf("Connection accepted from %s:%d\n", client_ip, ntohs(client_addr.sin_port));

    //send welcome message to client
    char *welcome_msg = "Welcome to the server!\n";
    send(client_fd, welcome_msg, strlen(welcome_msg), 0);
    printf("Welcome message sent\n");

    //receive file name from client
    memset(buffer, 0, sizeof(buffer));
    int bytes_received = recv(client_fd, buffer, sizeof(buffer)-1, 0);
    if(bytes_received < 0){
        perror("Receive failed");
        close(client_fd);
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    buffer[bytes_received] = '\0'; //null terminate the string
    printf("Requested file: %s\n", buffer);

    //open the requested file
    FILE *file = fopen(buffer, "rb");
    if(file == NULL){
        char *error_msg = "File not found\n";
        send(client_fd, error_msg, strlen(error_msg), 0);
        printf("File not found: %s\n", buffer);
    } else {
        //send file content to client
        char file_buffer[1024];
        int n;
        while((n = fread(file_buffer, 1, sizeof(file_buffer), file)) > 0){
            if(send(client_fd, file_buffer, n, 0) < 0){
                perror("Send failed");
                break;
            }
        }
        fclose(file);
        printf("File sent successfully\n");
    }
    close(client_fd);  
    close(server_fd);

    return 0;
}
