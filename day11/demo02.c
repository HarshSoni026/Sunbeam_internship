#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/socket.h>
#include<arpa/inet.h>

#define PORT 8081
#define IP "127.0.0.1"

int main(){
    int client_fd;
    struct sockaddr_in server_addr;
    char buffer[1024] = {0};
    char filename[256];

    client_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(client_fd < 0){
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }
    printf("Socket created successfully\n");
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr(IP);

    //connect to server
    if(connect(client_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0){
        perror("Connection to server failed");
        close(client_fd);
        exit(EXIT_FAILURE);
    }
    printf("Connected to server %s:%d\n", IP, PORT);
    //receive welcome message from server
    int valread = read(client_fd, buffer, sizeof(buffer)-1);
    if(valread < 0){
        perror("Read failed");
        close(client_fd);
        exit(EXIT_FAILURE);
    }
    //ask user for filename
    printf("Enter filename to request: ");
    scanf("%s", filename);
    //send filename to server
    send(client_fd, filename, strlen(filename), 0);
    send(client_fd, "\n", 1, 0); //send newline to indicate end of filename
    //receive file content from server
    FILE *fp = NULL;
    char save_as[300];
    snprintf(save_as, sizeof(save_as), "received_%s", filename);

    while((valread = recv(client_fd, buffer, sizeof(buffer)-1, 0)) > 0){
        buffer[valread] = '\0';
        if(strstr(buffer, "550 file not found") != NULL){
            printf("Server response: %s\n", buffer);
            break;
        }
        if(fp == NULL){
            fp = fopen(save_as, "wb");
            if(!fp){
                perror("File open error");
                break;
            }
            printf("Receiving file and saving as %s\n", save_as);
        }
        fwrite(buffer, 1, valread, fp);
    }
    if(fp != NULL){
        fclose(fp);
        printf("File received successfully\n");
    } else {
        printf("No file received\n");
    }
    close(client_fd);
    return 0;

}