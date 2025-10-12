#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

void* handle_client(void* arg) {
    int client = *(int*)arg;
    free(arg);

    char request[4096] = {0};
    read(client, request, sizeof(request));
    char method[16], path[256];
    sscanf(request, "%s %s", method, path);

    if (strcmp(path, "/") == 0) strcpy(path, "/index.html");

    if (strncmp(method, "POST", 4) == 0) {
        char* body = strstr(request, "\r\n\r\n");
        if (body) body += 4;

        FILE* f = fopen("data.txt", "a");
        if (f) { fprintf(f, "%s\n", body); fclose(f); }

        char response[] =
            "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\nPOST data saved";
        send(client, response, strlen(response), 0);
    } else {
        FILE* f = fopen(path+1, "r");
        if (!f) {
            char response[] =
                "HTTP/1.1 404 Not Found\r\nContent-Type: text/html\r\n\r\n<h1>404 Not Found</h1>";
            send(client, response, strlen(response), 0);
        } else {
            char header[] = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
            send(client, header, strlen(header), 0);
            char buf[1024];
            while (fgets(buf, sizeof(buf), f)) send(client, buf, strlen(buf), 0);
            fclose(f);
        }
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
    printf("Day 4: Final HTTP server running...\n");

    while (1) {
        int *client = malloc(sizeof(int));
        *client = accept(sockfd, NULL, NULL);
        pthread_t tid;
        pthread_create(&tid, NULL, handle_client, client);
        pthread_detach(tid);
    }
}
