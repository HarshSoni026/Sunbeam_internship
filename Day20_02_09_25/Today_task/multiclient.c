/* multi_client.c */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_IP "127.0.0.1" // Localhost
#define SERVER_PORT 8081       // Must match your server port
#define BUF_SIZE 1024

int main() {
    int sockfd;
    struct sockaddr_in serv_addr;
    char buffer[BUF_SIZE];
    int n;

    // 1. Create socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    // 2. Prepare server address
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(SERVER_PORT);

    // Convert and store IP address
    if (inet_pton(AF_INET, SERVER_IP, &serv_addr.sin_addr) <= 0) {
        perror("inet_pton");
        close(sockfd);
        exit(EXIT_FAILURE);
    }

    // 3. Connect to server
    if (connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("connect");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
    printf("Connected to server at %s:%d\n", SERVER_IP, SERVER_PORT);

    // 4. Send and receive loop
    while (1) {
        printf("Enter message (or 'quit' to exit): ");
        fgets(buffer, BUF_SIZE, stdin);
        if (strncmp(buffer, "quit", 4) == 0) break;

        // Send to server
        write(sockfd, buffer, strlen(buffer));

        // Read and display response
        n = read(sockfd, buffer, BUF_SIZE - 1);
        if (n <= 0) break;
        buffer[n] = '\0';
        printf("Server response: %s", buffer);
    }

    // 5. Clean up
    close(sockfd);
    printf("Disconnected.\n");
    return 0;
}
