// Add synchronization using a pthread_mutex to maintain a global counter of total messages handled across all clients.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 8080
#define BUF_SIZE 1024

// Global counter + mutex
int total_messages = 0;
pthread_mutex_t counter_mutex = PTHREAD_MUTEX_INITIALIZER;

void *handle_client(void *arg) {
    int client_fd = *(int*)arg;
    free(arg);

    char buffer[BUF_SIZE];
    int n;
    int client_msg_count = 0;  // per-client counter

    while ((n = read(client_fd, buffer, BUF_SIZE - 1)) > 0) {
        buffer[n] = '\0';
        client_msg_count++;

        // Update global counter safely
        pthread_mutex_lock(&counter_mutex);
        total_messages++;
        pthread_mutex_unlock(&counter_mutex);

        printf("Thread %lu received from client (%d msgs so far): %s\n",
               pthread_self(), client_msg_count, buffer);

        // Echo back
        write(client_fd, buffer, strlen(buffer));
    }

    printf("Thread %lu finished: Client sent %d messages. "
           "Total messages handled = %d\n",
           pthread_self(), client_msg_count, total_messages);

    close(client_fd);
    return NULL;
}

int main() {
    int server_fd, *client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) { perror("socket"); exit(1); }

    // Allow reuse of address
    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

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

    // Cleanup (won’t be reached in this infinite loop)
    pthread_mutex_destroy(&counter_mutex);

    return 0;
}

