#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<netinet/ip.h>

#define SERVER_PORT 2809
#define SERVER_IP "127.0.0.1"   

int main(){
    int server_fd, client_fd, ret;

    struct sockaddr_in server_addr, client_addr;
    socklen_t socklen;

    char str[64];

    //create socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(server_fd < 0){
        perror("failed to create listening socket");
        _exit(1);
    }

    //assign address to the server socket

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    inet_aton(SERVER_IP, &server_addr.sin_addr);
    ret = bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));

    if(ret < 0){
        perror("failed to bind server address to server sockcet");
        _exit(2);

    }

    //listen to server socket

    listen(server_fd, 5);

    //accept client connection

    memset(&client_addr, 0, sizeof(client_addr));

    socklen = sizeof(client_addr);
    client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &socklen);


    if(client_fd < 0){
        perror("failed to accept client connection");
        _exit(3);
    }

    do{
        //read from client
        read(client_fd, str, sizeof(str));
        printf("received from client: %s\n", str);

        printf("server:");
        gets(str);
        //write to client
        write(client_fd, str, sizeof(str));

    }while(strcmp(str, "bye") != 0);

    close(client_fd);
    shutdown(server_fd, SHUT_RDWR);

    return 0;
}