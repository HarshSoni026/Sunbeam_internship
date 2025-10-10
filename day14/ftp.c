#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/socket.h>
#include<arpa/inet.h>
#include<dirent.h>


#define PORT 8081
#define IP "127.0.0.1"


const char *FTP_user = "admin";
const char *FTP_pass = "1234";

void send_directory_listing(int client_fd){
    DIR *d;
    struct dirent *dir;
    char file_list[4096] = {0};
    d = opendir(".");
    if(d){
        while((dir = readdir(d)) != NULL){
            strcat(file_list, dir->d_name);
            strcat(file_list, "\r\n");
        }
        closedir(d);

    }
    else{
        strcpy(file_list, "failed to open directory\r\n");

    }
    write(client_fd, file_list, strlen(file_list));
}

int main(void){
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len;
    char buffer[1024];
    //create socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(server_fd < 0){
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr(IP);

    int opt = 1;
    if(setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))< 0){
        perror("Set socket options failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }
    printf("Socket created successfully\n");
    //bind socket
    if(bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr))<0){
        perror("bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }
    printf("Socket binded successfully\n");
    //listen
    if(listen(server_fd, 5) < 0){
        perror("listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }
    printf("Server listening on %s:%d\n", IP, PORT);
    //accept
    addr_len = sizeof(client_addr);
    client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &addr_len);

    write(client_fd, "220 welcome to my ftp server\r\n", 28);
    while(1){
        memset(buffer, 0, sizeof(buffer));
        int bytes = recv(client_fd, buffer, sizeof(buffer)-1, 0);
        if(bytes <= 0){
            break;
        }
        buffer[str]

    }
    return 0;
}