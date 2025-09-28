#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<dirent.h>

#define PORT 8080
#define IP "127.0.0.1"

int main(){
    int server_fd, client_fd;
    char BUFFER_SIZE[1024];   
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    char client_ip[INET_ADDRSTRLEN];

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(server_fd < 0){
        perror("Socket creation failed");
        exit(EXIT_FAILURE); 
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(IP);
    server_addr.sin_port = htons(PORT);

    if(bind(server_fd, (struct sockaddr*)&server_addr,sizeof(server_addr)) < 0){
        perror("Bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    listen(server_fd, 5);
    printf("Server listening on port %d\n", PORT);

    while(1){
        client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_addr_len);
        if(client_fd < 0){
            perror("Accept failed");
            continue;
        }
        printf("Client connected: %s\n", inet_ntoa(client_addr.sin_addr));

        inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, sizeof(client_ip));
        printf("Connected to client: %s:%d\n", client_ip, ntohs(client_addr.sin_port));

        char* Welcome_msg = "Welcome to the Server";
        if(write(client_fd, Welcome_msg, strlen(Welcome_msg)) < 0){
            perror("Write failed");
            close(client_fd);
            continue;
        }

        memset(BUFFER_SIZE, 0, sizeof(BUFFER_SIZE));
        int bytes_read = read(client_fd, BUFFER_SIZE, sizeof(BUFFER_SIZE)-1);
        if(bytes_read < 0){
            perror("Read failed");
            break;
        }

        if(strncmp(BUFFER_SIZE, "exit", 4) == 0){
            printf("Client requested to close the connection.\n");
            close(client_fd);
            break;
        }   
        printf("Client Says: %s\n", BUFFER_SIZE);
    }
    close(client_fd);
    close(server_fd);
    return 0;

}