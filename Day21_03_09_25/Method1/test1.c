// multi_client_fork.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8081
#define BUF_SIZE 1024

void handle_client(int client_fd) {
    char buffer[BUF_SIZE];
    int n;
    pid_t child_pid = getpid();

    printf("Child process started. PID: %d\n", child_pid);

    while ((n = read(client_fd, buffer, BUF_SIZE - 1)) > 0) {
        buffer[n] = '\0';
        buffer[strcspn(buffer, "\r\n")] = 0; // remove newline

        // Check for quit command
        if (strcasecmp(buffer, "quit") == 0) {
            char bye_msg[BUF_SIZE];
            snprintf(bye_msg, sizeof(bye_msg), "[PID %d] Goodbye!\n", child_pid);
            write(client_fd, bye_msg, strlen(bye_msg));
            break;  // exit the loop, close socket
        }

        printf("[PID %d] Received: %s\n", child_pid, buffer);

        char reply[BUF_SIZE];
        snprintf(reply, sizeof(reply), "[PID %d] Echo: %s\n", child_pid, buffer);
        write(client_fd, reply, strlen(reply));
    }

    printf("[PID %d] Client disconnected.\n", child_pid);
    close(client_fd);
    exit(0);
}


int main() {
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) { perror("socket"); exit(1); }

    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("setsockopt"); exit(1);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind"); exit(1);
    }

    listen(server_fd, 5);
    printf("Fork-based server listening on port %d...\n", PORT);

    while (1) {
        client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_len);
        if (client_fd < 0) { perror("accept"); continue; }

        pid_t pid = fork();
        if (pid == 0) {
            // Child process
            close(server_fd); 
            handle_client(client_fd);
        }
        else if (pid > 0) {
            // Parent process
            printf("Spawned child process. PID: %d\n", pid);
            close(client_fd); 
        }
        else {
            perror("fork");
            close(client_fd);
        }
    }

    close(server_fd);
    return 0;
}
