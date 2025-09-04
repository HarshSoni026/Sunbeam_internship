#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT       8080
#define BUF_SIZE   1024

// Credentials
#define USERNAME "admin"
#define PASSWORD "1234"

// --------- Utility Functions (Skeletons) ---------
void handle_list(int client_fd) {
    // TODO: Implement directory listing
    char *msg = "[LIST] Placeholder - send file list here\n";
    send(client_fd, msg, strlen(msg), 0);
}

void handle_get(int client_fd, char *filename) {
    // TODO: Implement file download
    char msg[BUF_SIZE];
    snprintf(msg, BUF_SIZE, "[GET] Placeholder - send file %s\n", filename);
    send(client_fd, msg, strlen(msg), 0);
}

void handle_put(int client_fd, char *filename) {
    // TODO: Implement file upload
    char msg[BUF_SIZE];
    snprintf(msg, BUF_SIZE, "[PUT] Placeholder - receive file %s\n", filename);
    send(client_fd, msg, strlen(msg), 0);
}

// --------- Authentication ---------
int authenticate_user(int client_fd) {
    char buffer[BUF_SIZE];
    char username[BUF_SIZE], password[BUF_SIZE];

    // Ask for username
    send(client_fd, "Enter username: ", strlen("Enter username: "), 0);
    memset(buffer, 0, BUF_SIZE);
    recv(client_fd, buffer, BUF_SIZE, 0);
    buffer[strcspn(buffer, "\n")] = 0;  // remove newline
    strcpy(username, buffer);

    // Ask for password
    send(client_fd, "Enter password: ", strlen("Enter password: "), 0);
    memset(buffer, 0, BUF_SIZE);
    recv(client_fd, buffer, BUF_SIZE, 0);
    buffer[strcspn(buffer, "\n")] = 0;  // remove newline
    strcpy(password, buffer);

    // Verify
    if (strcmp(username, USERNAME) == 0 && strcmp(password, PASSWORD) == 0) {
        send(client_fd, "Login successful! Welcome to FTP server.\n", 40, 0);
        printf("User '%s' logged in successfully.\n", username);
        return 1;
    } else {
        send(client_fd, "Login failed! Invalid credentials.\n", 35, 0);
        printf("Login attempt failed.\n");
        return 0;
    }
}

// --------- Main ---------
int main() {
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size;
    char buffer[BUF_SIZE];

    // 1. Create socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("Socket creation failed");
        exit(1);
    }

    // 2. Allow port reuse
    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    // 3. Configure server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // 4. Bind socket
    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        close(server_fd);
        exit(1);
    }

    // 5. Listen for connections
    listen(server_fd, 5);
    printf("FTP Server listening on port %d...\n", PORT);

    // 6. Accept a client
    addr_size = sizeof(client_addr);
    client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &addr_size);
    if (client_fd < 0) {
        perror("Accept failed");
        exit(1);
    }
    printf("Client connected.\n");

    // 7. Authenticate
    if (!authenticate_user(client_fd)) {
        close(client_fd);
        close(server_fd);
        return 0;
    }

    // 8. Command loop
    while (1) {
        memset(buffer, 0, BUF_SIZE);
        int n = recv(client_fd, buffer, BUF_SIZE, 0);
        if (n <= 0) break;

        buffer[strcspn(buffer, "\n")] = 0; // clean newline

        if (strncmp(buffer, "LIST", 4) == 0) {
            handle_list(client_fd);
        } else if (strncmp(buffer, "GET ", 4) == 0) {
            handle_get(client_fd, buffer + 4);
        } else if (strncmp(buffer, "PUT ", 4) == 0) {
            handle_put(client_fd, buffer + 4);
        } else if (strncmp(buffer, "QUIT", 4) == 0) {
            send(client_fd, "Goodbye!\n", 9, 0);
            printf("Client disconnected.\n");
            break;
        } else {
            send(client_fd, "Unknown command\n", 16, 0);
        }
    }

    // 9. Close connection
    close(client_fd);
    close(server_fd);
    return 0;
}

