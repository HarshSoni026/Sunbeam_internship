#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<unistd.h>
#include<netinet/in.h>
#include<arpa/inet.h>

#define PORT    8080
#define IP      "127.0.0.1"

int main()
{
    int server_fd, client_fd;
    int ret;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size;
    char buffer[1024];
    char client_ip[INET_ADDRSTRLEN];

    server_fd = socket(AF_INET,SOCK_STREAM,0);
    if(server_fd < 0)
    {
        perror("Socket creation failed!\n");
        exit(0);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr(IP);

    bind(server_fd,(struct sockaddr*)&server_addr,sizeof(server_addr));
}