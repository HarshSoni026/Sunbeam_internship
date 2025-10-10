#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<netinet/in.h>

int main(void){
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size;
    char client_ip[INET_ADDRSTRLEN];

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(server_fd < 0){
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(8080);

    memset(&(server_addr.sin_zero), '\0', 8);

    if(bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr))<0){
        perror("Bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }
    printf("Bind to port 8080 successful\n");
    if(listen(server_fd, 5)<0){
        perror("Listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }
    printf("Server listening on port 8080...\n");

    addr_size = sizeof(client_addr);
    client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &addr_size);
    if(client_fd < 0){
        perror("Accept failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }
    inet_ntop(AF_INET, &(client_addr.sin_addr), client_ip, INET_ADDRSTRLEN);
    printf("Connection accepted from %s:%d\n", client_ip, ntohs(client_addr.sin_port));

    close(client_fd);
    close(server_fd);
    return 0;
}