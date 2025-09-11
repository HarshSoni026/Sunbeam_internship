//Code to print the client message on server terminal and print a welcome message when connection is established.


#include <stdio.h>              
#include <stdlib.h>             
#include <string.h>             
#include <unistd.h>             
#include <sys/socket.h>         
#include <netinet/in.h>         
#include <arpa/inet.h>          

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

    inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, sizeof(client_ip));
    printf("Connected to client: %s:%d\n", client_ip, ntohs(client_addr.sin_port));

    // Task 2:- Modify the server to send a welcome message to the client immediately after connection. ( Hint :- use write() and accept() )
    char *welcome_msg = "Welcome";
    if(write(client_fd,welcome_msg,strlen(welcome_msg))<0){
        perror("Write to client failed!");
    }

    // Task 1:- Modify the server to print client messages on the server terminal.
   
    while(1){
        memset(buffer, 0, sizeof(buffer));
        int byte_received = recv(client_fd, buffer, sizeof(buffer),0);
        if(byte_received <= 0){
            printf("Client disconnected or error occurred.\n");
            break;
        }
         printf("Client: %s\n", buffer);
    }

    close(client_fd);
    close(server_fd);

    return 0;
}
