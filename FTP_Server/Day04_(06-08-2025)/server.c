#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>

int main() {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("Socket creation failed");
        return 1;
    }

    struct sockaddr_in server_addr = {0};
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(2121);

    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        return 1;
    }

    if (listen(server_fd, 3) < 0) {
        perror("Listen failed");
        return 1;
    }

    printf("Echo server running on port 2121\n");

    int client_fd = accept(server_fd, NULL, NULL);
    if (client_fd < 0) {
        perror("Accept failed");
        return 1;
    }

    printf("Client connected.\n");

    char buffer[1024];
    int bytes;

    while ((bytes = recv(client_fd, buffer, sizeof(buffer) - 1, 0)) > 0) {
        // Null-terminate the buffer to safely use with printf
        buffer[bytes] = '\0';

        // Print message to server terminal
        printf("Client says: %s\n", buffer);

        // Echo back to client
        //send(client_fd, buffer, bytes, 0);
    }

    if (bytes < 0) {
        perror("Receive error");
    } else {
        printf("Client disconnected.\n");
    }

    close(client_fd);
    close(server_fd);
    return 0;
}
