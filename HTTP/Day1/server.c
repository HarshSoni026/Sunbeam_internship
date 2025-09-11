#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) { perror("socket"); exit(1); }

    struct sockaddr_in addr = {0};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8080);
    addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_fd, (struct sockaddr*)&addr, sizeof(addr)) < 0) {
        perror("bind"); exit(1);
    }
    listen(server_fd, 5);
    printf("Day 1: HTTP server running on port 8080...\n");

    int client = accept(server_fd, NULL, NULL);
    char buffer[1024] = {0};
    read(client, buffer, sizeof(buffer)-1);

    printf("Request:\n%s\n", buffer);

    char response[] =
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html\r\n\r\n"
        "<h1>Hello, HTTP World!</h1>";

    send(client, response, strlen(response), 0);
    close(client);
    close(server_fd);
    return 0;
}
