#include<stdio.h>
#include<string.h>
#include<math.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/socket.h>
#include<arpa/inet.h>

#define PORT 8081
#define IP "127.0.0.1"

int main(void){

    int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(socket_fd < 0){
        perror("socket creation failed");
        exit(EXIT_FAILURE);

    }
    struct sockaddr_in server_addr;  //structure to hold server address
    server_addr.sin_family = AF_INET; //IPv4
    server_addr.sin_port = htons(PORT); //port number
    server_addr.sin_addr.s_addr = INADDR_ANY; //IP address

    memset(&(server_addr.sin_zero), 0, 8); //zero the rest of the struct


    //3. Bind the socket to the address and port number
    if(bind(socket_fd, (struct sockaddr*)&server_addr, sizeof(server_addr))<0){
        //if bind fails 
        perror("bind failed");
        close(socket_fd);
        exit(EXIT_FAILURE);
    }

    printf("Socket successfully binded to IP=%s:PORT=%d\n", IP, PORT);  
    close(socket_fd);
    return 0;
}