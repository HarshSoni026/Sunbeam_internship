#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/socket.h>
#include<arpa/inet.h>

#define PORT 8080
#define IP "127.0.0.1"

int main(){
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(server_fd < 0){
        perror("server_fd failed.");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in server_addr, client_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;   //accept connection on any IP

    memset(&(server_addr.sin_zero), 0, 8);

    //bind the socket to the specified IP and PORT
    if(bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0){
        perror("bind failed.");
        close(server_fd);
        exit(1);
    }

    //start listening for incoming connections
    listen(server_fd, 5);
    printf("Server listening to port : %d\n", PORT);

    //accept an incoming client connection

    socklen_t addr_size;
    char client_ip[INET_ADDRSTRLEN];
    char buffer[1024];

    addr_size = sizeof(client_addr);
    int client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &addr_size);
    if(client_fd < 0){
        perror("accept failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    inet_ntop(AF_INET, &(client_addr.sin_addr), client_ip, INET_ADDRSTRLEN);
    printf("Client connected from %s:%d\n", client_ip, ntohs(client_addr.sin_port));

    //communication receive msg from client

    memset(buffer, 0, sizeof(buffer)); //clear buffer before receiving.
    recv(client_fd, buffer, sizeof(buffer), 0);

    printf("client says : %s\n", buffer);

    //send a response back to client

    char *reply = "Hello from server!";
    send(client_fd, reply, strlen(reply), 0);

    close(client_fd);
    close(server_fd);

    return 0;
}