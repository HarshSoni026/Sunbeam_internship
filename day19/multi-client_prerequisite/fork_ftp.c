#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 2121
#define BUFFER_SIZE 1024

void handle_client(int client_fd){
    char buffer[BUFFER_SIZE];
    int n;
    while((n = read(client_fd, buffer, BUFFER_SIZE - 1)) >0){
        buffer[n] = '\0';
        printf("child pid : %d Received: %s\n", getpid(),  buffer);

        write(client_fd, buffer, strlen(buffer));

    }
    close(client_fd);

}
int main(void){
    int server_fd, client_fd;
    struct sockaddr_in server_addr;
    socklen_t addr_len = sizeof(server_addr);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(server_fd < 0){
        perror("Socket creation failed");
        exit(EXIT_FAILURE); 
    }
    int opt = 1;
    if(setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0){
        perror("Set socket options failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }
    memset(&server_addr, 0, sizeof(server_addr));

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);
    if(bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0){
        perror("Bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }
    if(listen(server_fd, 5) < 0){
        perror("Listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }
    printf("Server listening on port %d\n", PORT);
    
    while(1){
        client_fd = accept(server_fd, (struct sockaddr*)&server_addr, &addr_len);
        if(fork() == 0){
            close(server_fd);
            handle_client(client_fd);
            exit(0);
        }
        close(client_fd);
    }
    
    return 0;
}