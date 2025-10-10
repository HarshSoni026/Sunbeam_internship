#include<stdio.h>
#include<string.h> //
#include<stdlib.h>  //for exit()
#include<unistd.h> // for close(), read(), write()
#include<arpa/inet.h> // for inet_ntop() to convert IP addresses to readable form
#include<sys/socket.h> //for socket(), bind(), listen(), accept()
#include<netinet/in.h> // for sockaddr_in and IP related structures

#define PORT 8081
#define IP "127.0.0.1"


int main(void){
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
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(IP);
    server_addr.sin_port = htons(PORT);

    //reuse port immidietely after program termination
    int opt = 1;
    if(setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0){
        perror("setsockopt failed");
        exit(EXIT_FAILURE);
    }
    if(bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0){
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }
    listen(server_fd, 3);
    printf("Listening to PORT : %d\n", PORT);

    addr_size = sizeof(client_addr);

    client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &addr_size);
    if(client_fd < 0){
        perror("accept failed");
        exit(EXIT_FAILURE);
    }

    //receive file name from client
    memset(buffer, 0, sizeof(buffer));
    int bytes = read(client_fd, buffer, sizeof(buffer)- 1);
    if(bytes < 0){
        printf("no filename received\n");
        close(client_fd);
        close(server_fd);
        return 0;
    }
    buffer[bytes] = '\0';

    buffer[strcspn(buffer, "\r\n")] = 0;
    printf("File requested: %s\n", buffer);

    //open the requested file

    FILE *fp = fopen(buffer, "rb");
    if(fp == NULL){
        char *error_msg = "550 : File not found\r\n";
        send(client_fd, error_msg, strlen(error_msg), 0);

    }
    else{
        //send file content to the client
        char file_buffer[1024];
        int n;
        while((n = fread(file_buffer, 1, sizeof(file_buffer), fp)) > 0){
            if(send(client_fd, file_buffer, n, 0) < 0){
                perror("File send error");
                break;
            }

        }
        fclose(fp);
        printf("File transfer completed.\n");

    }

    close(client_fd);
    close(server_fd);


    return 0;
}