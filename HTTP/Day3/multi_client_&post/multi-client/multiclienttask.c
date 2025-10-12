#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <time.h>

struct client_info {
    int sock;
    struct sockaddr_in addr;
};

void log_request(const char* client_ip, const char* file) {
    FILE* f = fopen("access.log", "a");
    if (!f) return;
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    char timestr[64];
    strftime(timestr, sizeof(timestr), "%Y-%m-%d %H:%M:%S", t);
    fprintf(f, "[%s] %s requested %s\n", timestr, client_ip, file);
    fclose(f);
}

void* handle_client(void* arg) {
    struct client_info info = *(struct client_info*)arg;
    free(arg);
    int client = info.sock;
    char client_ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(info.addr.sin_addr), client_ip, sizeof(client_ip));

    char request[2048] = {0};
    read(client, request, sizeof(request));

    char method[16], path[256];
    sscanf(request, "%s %s", method, path);

    if (strcmp(path, "/") == 0)
        strcpy(path, "/index.html");

    log_request(client_ip, path);

    // Remove leading '/'
    const char* file_path = path + 1;
    FILE* f = fopen(file_path, "r");

    if (!f) {
        char response[] =
            "HTTP/1.1 404 Not Found\r\nContent-Type: text/html\r\n\r\n"
            "<h1>404 Not Found</h1>";
        send(client, response, strlen(response), 0);
    } else {
        // Detect file type (basic)
        char* ext = strrchr(file_path, '.');
        const char* content_type = "text/plain";
        if (ext) {
            if (strcmp(ext, ".html") == 0) content_type = "text/html";
            else if (strcmp(ext, ".css") == 0) content_type = "text/css";
            else if (strcmp(ext, ".js") == 0) content_type = "application/javascript";
        }

        char header[256];
        snprintf(header, sizeof(header),
                 "HTTP/1.1 200 OK\r\nContent-Type: %s\r\n\r\n", content_type);
        send(client, header, strlen(header), 0);

        char buf[1024];
        while (fgets(buf, sizeof(buf), f))
            send(client, buf, strlen(buf), 0);
        fclose(f);
    }

    close(client);
    return NULL;
}

int main() {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in addr = {0};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(8080);
    addr.sin_addr.s_addr = INADDR_ANY;

    bind(sockfd, (struct sockaddr*)&addr, sizeof(addr));
    listen(sockfd, 10);

    printf("🚀 Multi-client server running on port 8080...\n");

    while (1) {
        struct client_info* info = malloc(sizeof(struct client_info));
        socklen_t len = sizeof(info->addr);
        info->sock = accept(sockfd, (struct sockaddr*)&info->addr, &len);
        pthread_t tid;
        pthread_create(&tid, NULL, handle_client, info);
        pthread_detach(tid);
    }
}

