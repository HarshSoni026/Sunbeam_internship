#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/stat.h>

void send_file(int client, const char *filename) {
    int fd = open(filename, O_RDONLY);
    if (fd < 0) {
        char notfound[] =
            "HTTP/1.1 404 Not Found\r\n"
            "Content-Type: text/html\r\n\r\n"
            "<h1>404 File Not Found</h1>";
        send(client, notfound, strlen(notfound), 0);
        return;
    }
    char header[] = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
    send(client, header, strlen(header), 0);

    char buf[1024];
    int n;
    while ((n = read(fd, buf, sizeof(buf))) > 0) {
        send(client, buf, n, 0);
    }
    close(fd);
}

int main() {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addr = {0};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8080);
    addr.sin_addr.s_addr = INADDR_ANY;

    bind(sockfd, (struct sockaddr*)&addr, sizeof(addr));
    listen(sockfd, 5);
    printf("Day 2: File-serving HTTP Server running on port 8080...\n");

    while (1) {
        int client = accept(sockfd, NULL, NULL);
        char request[1024] = {0};
        read(client, request, sizeof(request));
        char method[16], path[256];
        sscanf(request, "%s %s", method, path);

        if (strcmp(path, "/") == 0) strcpy(path, "/index.html");
        send_file(client, path+1); // remove leading '/'
        close(client);
    }
}
