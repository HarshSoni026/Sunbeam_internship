#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>

#include<sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<netinet/ip.h>

#define IP "127.0.0.1"
#define PORT 8081

int main(void){
    int client_fd;
    struct sockaddr_in client_addr;

    char buffer[1024];
    char file_name[256];

    //create socket.
    client_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(client_fd < 0){
        perror("socket failed!");
        exit(EXIT_FAILURE);
    }

    //server details 
    client_addr.sin_family = AF_INET;
    client_addr.sin_port = htons(PORT);
    client_addr.sin_addr.s_addr = inet_addr(IP);

    //connect to server
    if(connect(client_fd, (struct sockaddr*)&client_addr, sizeof(client_addr)) < 0){
        perror("connect failed!");
        close(client_fd);
        exit(EXIT_FAILURE);
    }

    //read welcome message from server
    int n = recv(client_fd, buffer, sizeof(buffer)-1,0);
    if(n < 0){
        perror("recv failed!");
        close(client_fd);
        exit(EXIT_FAILURE);
    }
    if(n>0){
        buffer[n] = '\0'; //null terminate the string
        printf("Server: %s", buffer);
    }   

    //ask user for file name
    printf("Enter the file name to request:");
    scanf("%s", file_name);

    //send file name to server
    send(client_fd, file_name, strlen(file_name), 0);
    send(client_fd, "\n", 1, 0); //send newline to indicate end of file name as in telnet.


    //receive file response from server
    // FILE *fp = fopen("received_file", "wb");
    FILE *fp = NULL;
    char save_as[512];
    snprintf(save_as, sizeof(save_as), "received_%s", file_name);

    while((n = recv(client_fd, buffer, sizeof(buffer), 0))>0){
        buffer[n] = '\0';
        //check for error message
        if(strstr(buffer, "550 file not found") != NULL){
            printf("server: %s", buffer);
            break;
        }

        //open file for writing if not already opened
        if(fp ==NULL){
            fp = fopen(save_as, "wb");
            if(fp == NULL){
                perror("fopen failed!");
                break;
            }
            printf("saving file as %s\n", save_as);


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