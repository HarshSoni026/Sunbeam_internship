#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <dirent.h>

#define PORT 2121
#define IP   "127.0.0.1"

const char *FTP_USER = "admin";
const char *FTP_PASS = "1234";

void send_directory_listing(int client_fd) {
    DIR *d;
    struct dirent *dir;
    char file_list[4096] = "";

    d = opendir(".");
    if (d) {
        while ((dir = readdir(d)) != NULL) {
            strcat(file_list, dir->d_name);
            strcat(file_list, "\r\n");
        }
        closedir(d);
    } else {
        strcpy(file_list, "Failed to open directory.\r\n");
    }

    write(client_fd, file_list, strlen(file_list));
}

int main() {
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size;
    char buffer[1024];
    int logged_in = 0;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("Socket creation failed");
        exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr(IP);

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        exit(1);
    }

    listen(server_fd, 5);
    printf("FTP server running on %s:%d...\n", IP, PORT);

    addr_size = sizeof(client_addr);
    client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &addr_size);

    write(client_fd, "220 Welcome to Mini FTP Server\r\n", 32);

    while (1) {
        memset(buffer, 0, sizeof(buffer));
        int bytes = recv(client_fd, buffer, sizeof(buffer) - 1, 0);
        if (bytes <= 0) break;

        buffer[strcspn(buffer, "\r\n")] = 0; // remove CRLF

        if (strncmp(buffer, "USER ", 5) == 0) {
            if (strcmp(buffer + 5, FTP_USER) == 0) {
                write(client_fd, "331 Username OK, need password\r\n", 32);
            } else {
                write(client_fd, "530 Invalid username\r\n", 22);
            }
        }
        else if (strncmp(buffer, "PASS ", 5) == 0) {
            if (strcmp(buffer + 5, FTP_PASS) == 0) {
                logged_in = 1;
                write(client_fd, "230 Login successful\r\n", 22);
            } else {
                write(client_fd, "530 Invalid password\r\n", 22);
            }
        }
        else if (strncmp(buffer, "LIST", 4) == 0) {
            if (!logged_in) {
                write(client_fd, "530 Please login first\r\n", 24);
            } else {
                write(client_fd, "150 Here comes the directory listing\r\n", 38);
                send_directory_listing(client_fd);
                write(client_fd, "226 Directory send OK\r\n", 24);
            }
        }
        else if (strncmp(buffer, "QUIT", 4) == 0) {
            write(client_fd, "221 Goodbye\r\n", 13);
            break;
        }
        else {
            write(client_fd, "502 Command not implemented\r\n", 30);
        }
    }

    close(client_fd);
    close(server_fd);
    return 0;
}
