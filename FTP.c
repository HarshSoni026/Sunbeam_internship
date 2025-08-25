#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <dirent.h>

#define CONTROL_PORT 2121
#define IP "127.0.0.1"

const char *FTP_USER = "admin";
const char *FTP_PASS = "1234";

void send_directory_listing(int data_fd) {
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

    write(data_fd, file_list, strlen(file_list));
}

int start_data_socket(int *port) {
    int data_fd;
    struct sockaddr_in data_addr;

    data_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (data_fd < 0) {
        perror("Data socket creation failed");
        return -1;
    }

    data_addr.sin_family = AF_INET;
    data_addr.sin_addr.s_addr = inet_addr(IP);
    data_addr.sin_port = 0; // Let OS choose random port

    int opt = 1;
    setsockopt(data_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    if (bind(data_fd, (struct sockaddr*)&data_addr, sizeof(data_addr)) < 0) {
        perror("Data bind failed");
        close(data_fd);
        return -1;
    }

    socklen_t len = sizeof(data_addr);
    getsockname(data_fd, (struct sockaddr*)&data_addr, &len);
    *port = ntohs(data_addr.sin_port);

    listen(data_fd, 1);
    return data_fd;
}

int main() {
    int control_fd, client_fd;
    struct sockaddr_in control_addr, client_addr;
    socklen_t addr_size;
    char buffer[1024];
    int logged_in = 0;

    control_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (control_fd < 0) {
        perror("Control socket creation failed");
        exit(1);
    }

    control_addr.sin_family = AF_INET;
    control_addr.sin_port = htons(CONTROL_PORT);
    control_addr.sin_addr.s_addr = inet_addr(IP);

    int opt = 1;
    setsockopt(control_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    if (bind(control_fd, (struct sockaddr*)&control_addr, sizeof(control_addr)) < 0) {
        perror("Bind failed");
        exit(1);
    }

    listen(control_fd, 5);
    printf("FTP server (Passive mode) running on %s:%d...\n", IP, CONTROL_PORT);

    addr_size = sizeof(client_addr);
    client_fd = accept(control_fd, (struct sockaddr*)&client_addr, &addr_size);

    write(client_fd, "220 Welcome to Mini FTP Server (Passive Mode)\r\n", 47);

    int pasv_fd = -1, pasv_port = 0;

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
        else if (strncmp(buffer, "PASV", 4) == 0) {
            if (!logged_in) {
                write(client_fd, "530 Please login first\r\n", 24);
                continue;
            }
            pasv_fd = start_data_socket(&pasv_port);
            if (pasv_fd < 0) {
                write(client_fd, "425 Can't open data connection\r\n", 32);
                continue;
            }
            int p1 = pasv_port / 256;
            int p2 = pasv_port % 256;
            char reply[100];
            snprintf(reply, sizeof(reply), "227 Entering Passive Mode (127,0,0,1,%d,%d)\r\n", p1, p2);
            write(client_fd, reply, strlen(reply));
        }
        else if (strncmp(buffer, "LIST", 4) == 0) {
            if (!logged_in || pasv_fd < 0) {
                write(client_fd, "425 Use PASV first\r\n", 21);
                continue;
            }
            write(client_fd, "150 Opening data connection\r\n", 29);
            struct sockaddr_in data_client_addr;
            socklen_t data_size = sizeof(data_client_addr);
            int data_client_fd = accept(pasv_fd, (struct sockaddr*)&data_client_addr, &data_size);
            send_directory_listing(data_client_fd);
            close(data_client_fd);
            close(pasv_fd);
            pasv_fd = -1;
            write(client_fd, "226 Transfer complete\r\n", 24);
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
    close(control_fd);
    return 0;
}
