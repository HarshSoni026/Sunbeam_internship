#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>

#define PORT 8081
#define IP "127.0.0.1"

int main(void){
    int client_fd;
    struct sockaddr_in server_addr;
    char buffer[1024];
    char filename[256];

    //create socket
    client_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(client_fd < 0){
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    //server details
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr(IP);

    //connect to server

    if(connect(client_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0){
        perror("Connection to server failed");
        close(client_fd);
        exit(EXIT_FAILURE);

    }


    printf("Connected to the server at %s:%d\n", IP, PORT);

    //read welcome message from server
    int n = recv(client_fd, buffer, sizeof(buffer)-1, 0);
    if(n > 0){
        buffer[n] = '\0';
        printf("Server: %s\n", buffer);

    }

    //ask user for filename
    printf("Enter the filename to request:");
    scanf("%s", filename);
    //send filename to server
    send(client_fd, filename, strlen(filename), 0);
    send(client_fd, "\n", 1, 0);

    //receive response from server
    FILE *fp = NULL;
    char save_as[256];
    snprintf(save_as, sizeof(save_as), "received %s", filename);

    while((n = recv(client_fd, buffer, sizeof(buffer)-1, 0))>0){
        buffer[n] = '\0';

        //check for error message
        if(strstr(buffer, "550: file not found") != NULL){
            printf("Server: %s\n", buffer);
            break;

        }
        //open file for writing if not already opened
        if(fp == NULL){
            fp = fopen(save_as, "wb");
            if(!fp){
                perror("fopen failed");
                close(client_fd);
                exit(1);
            }
            printf("Saving file as : %s\n", save_as);

        }
        fwrite(buffer, 1, n, fp);

    }
    if(fp != NULL){
        fclose(fp);
            printf("file download completed.\n");

    }
    close(client_fd);
      
    

    return 0;
}