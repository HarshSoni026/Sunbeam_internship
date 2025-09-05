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
    pid_t pid = fork();
if (pid < 0) {
    perror("fork failed");
} else if (pid == 0) {
    // Child process
    pid_t child_pid = getpid();  // Correct: actual PID of this child
    printf("Child process started. PID: %d\n", child_pid);
    handle_client(client_fd);
    exit(0);
} else {
    // Parent process
    printf("Spawned a child process. PID: %d\n", pid);
    close(client_fd);
}


    close(client_fd);
}

int main() {
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) { perror("socket"); exit(1); }

    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
    perror("setsockopt");
    exit(1);
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

        if (fork() == 0) {
            close(server_fd); // child doesn’t need server socket
            handle_client(client_fd);
            exit(0);
        }
        close(client_fd); // parent closes client socket
    }

    return 0;
}

