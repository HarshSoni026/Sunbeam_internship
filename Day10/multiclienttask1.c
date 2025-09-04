// Modify handle_client() to count total messages per client and print them before closing the connection.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 8080
#define BUF_SIZE 1024

void *handle_client(void *arg) {
    int client_fd = *(int*)arg;
    free(arg);

    char buffer[BUF_SIZE];
    int n;
    int msg_count = 0;   // count messages per client

    while ((n = read(client_fd, buffer, BUF_SIZE - 1)) > 0) {
        buffer[n] = '\0';
        msg_count++;  // increment per message

        printf("Thread %lu received (%d): %s\n", pthread_self(), msg_count, buffer);

        // Echo back
        write(client_fd, buffer, strlen(buffer));
    }

    printf("Thread %lu finished: Total messages from client = %d\n",
           pthread_self(), msg_count);

    close(client_fd);
    return NULL;
}

int main() {
    int server_fd, *client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) { perror("socket"); exit(1); }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind"); exit(1);
    }

    listen(server_fd, 5);
    printf("Pthread-based server listening on port %d...\n", PORT);

    while (1) {
        client_fd = malloc(sizeof(int));
        *client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_len);
        if (*client_fd < 0) { perror("accept"); free(client_fd); continue; }

        pthread_t tid;
        pthread_create(&tid, NULL, handle_client, client_fd);
        pthread_detach(tid); // auto cleanup
    }

    return 0;
}

