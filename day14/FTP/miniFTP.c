#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <dirent.h>
#include <netinet/in.h>

#define PORT 2121
#define IP "127.0.0.1"

const char *USER = "admin";
const char *PASS = "1234";

void send_directory_listing(int client_fd){
    DIR *d;
    struct dirent *dir;
    char file_list[1024] = {0};

    d = opendir(".");
    if(d){
        while((dir = readdir(d)) != NULL){
            strcat(file_list, dir->d_name);
            strcat(file_list, "\r\n");

        }
        closedir(d);

    }
    else{
        strcpy(file_list, "Failed to open directory\r\n");
    }
    write(client_fd, file_list, strlen(file_list));


}

int main(){
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size;
    char buffer[1024] = {0};
    int logged_in = 0;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(IP);
    server_addr.sin_port = htons(PORT);


    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    if(bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr))<0){
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }
    listen(server_fd, 3);
    printf("FTP server listening on %s:%d\n", IP, PORT);
    addr_size = sizeof(client_addr);
    client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &addr_size);

    write(client_fd, "220 welcome to mini FTP server\r\n", 30);

    while(1){
        memset(buffer,0, sizeof(buffer));
        int bytes_read = recv(client_fd, buffer, sizeof(buffer)-1, 0);
        if(bytes_read <= 0) break;
        buffer[strcspn(buffer, "\r\n")] = 0;

        if(strncmp(buffer, "USER ", 5) == 0){
            if(strcmp(buffer + 5, USER) == 0){
                write(client_fd, "331 Username OK, need password\r\n", 33);
            }
            else{
                write(client_fd, "530 Invalid username\r\n", 22);
            }

        }
        else if(strncmp(buffer, "PASS ", 5) == 0){
            if(strcmp(buffer + 5, PASS) == 0){
                logged_in = 1;
                write(client_fd, "230 User logged in successfully\r\n", 32);

            }
            else{
                write(client_fd, "530 Invalid password\r\n", 22);
            }
        }
        else if(strncmp(buffer, "LIST", 4) == 0){
            if(!logged_in){
                write(client_fd, "530 please login in first \r\n", 28);

            }
            else{
                write(client_fd, "150 Here comes the directory listing\r\n", 39);
                send_directory_listing(client_fd);
                write(client_fd, "226 Directory send OK\r\n", 23);
            }
        }
        else if(strncmp(buffer, "QUIT", 4) == 0){
            write(client_fd, "221 Goodbye Exiting\r\n", 20);
            break;
        }
        else{
            write(client_fd, "502 Command not implemented\r\n", 30);

        }
    }

    close(client_fd);
    close(server_fd);



    return 0;
}