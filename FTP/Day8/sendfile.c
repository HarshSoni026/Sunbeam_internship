#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8080

int main() {
    int client_fd;
    struct sockaddr_in server_addr;
    char buffer[1024];
    char filename[256];

    // Create socket
    client_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (client_fd < 0) {
        perror("Socket creation failed");
        exit(1);
    }

    // Server details
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

    // Connect to server
    if (connect(client_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        close(client_fd);
        exit(1);
    }

    // Read welcome message
    int n = recv(client_fd, buffer, sizeof(buffer) - 1, 0);
    if (n > 0) {
        buffer[n] = '\0';
        printf("Server: %s\n", buffer);
    }

    while(1){
        // Ask user for filename
        printf("Enter filename to request: ");
        scanf("%s", filename);

        // Send filename
        send(client_fd, filename, strlen(filename), 0);
        send(client_fd, "\n", 1, 0); // send newline like telnet does

        // Receive response
        FILE *fp = NULL;
        char save_as[300];
        snprintf(save_as, sizeof(save_as), "received_%s", filename);

        while ((n = recv(client_fd, buffer, sizeof(buffer), 0)) > 0) {
        buffer[n] = '\0';

        // Check for error message
        if (strstr(buffer, "550 File not found") != NULL) {
            printf("Server: %s\n", buffer);
            break;
        }

        // Open file for writing if not already opened
        if (fp == NULL) {
            fp = fopen(save_as, "wb");
            if (!fp) {
                perror("fopen");
                close(client_fd);
                exit(1);
            }
            printf("Saving file as: %s\n", save_as);
        }

        fwrite(buffer, 1, n, fp);
    }

        if (fp != NULL) {
        fclose(fp);
        printf("File download completed.\n");
    }
         if (n == 0){
            break;
    }

    }

    close(client_fd);
    return 0;
}
