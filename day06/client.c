#include<stdio.h>
#include<stdint.h>
#include<unistd.h>
#include<string.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<netinet/ip.h>
#define SERVER_PORT 2809
#define SERVER_IP "127.0.0.1"  
int main(void){
    int client_fd, ret;

    struct sockaddr_in server_addr;

    char str[64];

    client_fd = socket(AF_INET, SOCK_STREAM, 0);

    if(client_fd < 0){
        perror("failed to create client socket");
        _exit(1);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    inet_aton(SERVER_IP, &server_addr.sin_addr);

    server_addr.sin_port = htons(SERVER_PORT);

    ret = connect(client_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));

    if(ret < 0){
        perror("failed to connect to server");
        _exit(2);
    }

    do{
        printf("client: ");
        gets(str);

        write(client_fd, str, strlen(str));

        //read from server
        read(client_fd, str, sizeof(str));
        printf("server: %s\n", str);

    }while(strcmp(str, "bye") != 0);

    close(client_fd);
    return 0;
}