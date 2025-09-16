#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <sys/stat.h>

void send_file(int client, const char *filename, int notfound) {
    int fd = open(filename, O_RDONLY);
    if (fd < 0) {
        if (notfound) {
            // fallback inline 404
            char notfound_msg[] =
                "HTTP/1.1 404 Not Found\r\n"
                "Content-Type: text/html\r\n\r\n"
                "<h1>404 File Not Found</h1>";
            send(client, notfound_msg, strlen(notfound_msg), 0);
        } else {
            // try serving 404.html if requested file not found
            send_file(client, "404.html", 1);
        }
        return;
    }

    // success response
    char header[] =
        "HTTP/1.1 200 OK\r\n"
        "Content-Type: text/html\r\n\r\n";
    send(client, header, strlen(header), 0);

    char buf[1024];
    int n;
    while ((n = read(fd, buf, sizeof(buf))) > 0) {
        send(client, buf, n, 0);
    }
    close(fd);
}

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
    printf("Day 2: File-serving HTTP server running on port 8080...\n");

    while (1) {
        int client = accept(server_fd, NULL, NULL);
        if (client < 0) { perror("accept"); continue; }

        char buffer[1024] = {0};
        read(client, buffer, sizeof(buffer)-1);
        printf("Request:\n%s\n", buffer);

        char method[16], path[256];
        sscanf(buffer, "%s %s", method, path);

        if (strcmp(path, "/") == 0) {
            strcpy(path, "/index.html");  // default page
        }

        // remove leading '/' to map to local file
        send_file(client, path + 1, 0);

        close(client);
    }

    close(server_fd);
    return 0;
}

