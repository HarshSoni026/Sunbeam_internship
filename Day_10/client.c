#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 8080
#define IP   "127.0.0.1"

int main() {
    int client_fd;
    struct sockaddr_in server_addr;
    char buffer[2048];
    char input[1024];

    // 1. Create TCP socket
    client_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (client_fd < 0) {
        perror("Socket creation failed!");
        exit(1);
    }

    // 2. Server address setup
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr(IP);

    // 3. Connect to server
    if (connect(client_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed!");
        close(client_fd);
        exit(2);
    }

    // 4. Receive welcome message
    memset(buffer, 0, sizeof(buffer));
    int bytes = recv(client_fd, buffer, sizeof(buffer), 0);
    if (bytes > 0) {
        printf("Server: %s", buffer);
    }

    // 5. Chat loop
    while (1) {
        printf("Enter command : ");
        fgets(input, sizeof(input), stdin);

        // Remove newline
        input[strcspn(input, "\r\n")] = 0;

        // Send to server
        if (send(client_fd, input, strlen(input), 0) < 0) {
            perror("Send failed!");
            break;
        }

        // If exit command, break
        if (strcasecmp(input, "bye") == 0 || strcasecmp(input, "exit") == 0) {
            printf("Closing connection.\n");
            break;
        }

        // Receive response from server
        memset(buffer, 0, sizeof(buffer));
        bytes = recv(client_fd, buffer, sizeof(buffer), 0);
        if (bytes > 0) {
            printf("Server: \n%s\n", buffer);
        } else {
            printf("Server closed connection.\n");
            break;
        }
    }

    close(client_fd);
    return 0;
}
