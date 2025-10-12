#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <time.h>

void log_request(const char* method, const char* path) {
    FILE* f = fopen("access.log", "a");
    time_t now = time(NULL);
    char* time_str = ctime(&now);
    time_str[strlen(time_str)-1] = '\0';
    fprintf(f, "[%s] %s request for %s\n", time_str, method, path);
    fclose(f);
}

void* handle_client(void* arg) {
    int client = *(int*)arg;
    free(arg);

    char request[8192] = {0};
    int bytes = recv(client, request, sizeof(request) - 1, 0);
    if (bytes <= 0) {
        close(client);
        return NULL;
    }

    char method[16], path[256];
    sscanf(request, "%s %s", method, path);

    if (strcmp(path, "/") == 0) strcpy(path, "/index.html");
    log_request(method, path);

    // ----- POST Handler -----
    if (strncmp(method, "POST", 4) == 0) {
        // Special handling for /form
        if (strcmp(path, "/form") == 0) {
            char* body = strstr(request, "\r\n\r\n");
            if (body) body += 4; // skip headers

            FILE* f = fopen("data.txt", "a");
            if (f && body) {
                fprintf(f, "%s\n", body);
                fclose(f);
            }

            char response[] =
                "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\nPOST data saved";
            send(client, response, strlen(response), 0);
            close(client);
            return NULL;
        }
    }

    // ----- GET Handler -----
    else if (strncmp(method, "GET", 3) == 0) {
        char *content_type = "text/html";
        if (strstr(path, ".css")) content_type = "text/css";

        FILE* f = fopen(path + 1, "r");
        if (!f) {
            char response[] =
                "HTTP/1.1 404 Not Found\r\nContent-Type: text/html\r\n\r\n<h1>404 Not Found</h1>";
            send(client, response, strlen(response), 0);
        } else {
            char header[128];
            sprintf(header, "HTTP/1.1 200 OK\r\nContent-Type: %s\r\n\r\n", content_type);
            send(client, header, strlen(header), 0);
            char buf[1024];
            while (fgets(buf, sizeof(buf), f))
                send(client, buf, strlen(buf), 0);
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
    printf("🚀 HTTP Server running on port 8080...\n");

    while (1) {
        int *client = malloc(sizeof(int));
        *client = accept(sockfd, NULL, NULL);
        pthread_t tid;
        pthread_create(&tid, NULL, handle_client, client);
        pthread_detach(tid);
    }
}

