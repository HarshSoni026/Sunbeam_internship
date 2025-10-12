#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <time.h>

void log_request(const char* file) {
    FILE* f = fopen("access.log", "a");
    time_t now = time(NULL);
    fprintf(f, "[%s] requested %s\n", ctime(&now), file);
    fclose(f);
}

void* handle_client(void* arg) {
    int client = *(int*)arg;
    free(arg);

    char request[1024] = {0};
    read(client, request, sizeof(request));
    char method[16], path[256];
    sscanf(request, "%s %s", method, path);

    if (strcmp(path, "/") == 0) strcpy(path, "/index.html");
    log_request(path);

    char response[] =
        "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n"
        "<h1>Multi-client HTTP Server</h1>";
    send(client, response, strlen(response), 0);

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
    printf("Day 3: Multi-client server running...\n");

    while (1) {
        int *client = malloc(sizeof(int));
        *client = accept(sockfd, NULL, NULL);
        pthread_t tid;
        pthread_create(&tid, NULL, handle_client, client);
        pthread_detach(tid);
    }
}
