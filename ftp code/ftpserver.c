#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT    8080
#define IP      "127.0.0.1"

//credentials
#define USERNAME "admin"
#define PASSWORD "1234"

int main() {
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    //socklen_t addr_len = sizeof(client_addr);
    socklen_t addr_size;
    char buffer[1024];
    char client_ip[INET_ADDRSTRLEN];

    // Create socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("Socket creation failed");
        //exit(1);
        exit(0);
    }

    // Configure server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    //server_addr.sin_addr.s_addr = INADDR_ANY;
    

    // Bind socket
    if (bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        close(server_fd);
        exit(1);
    }

    // Listen for clients
    if (listen(server_fd, 5) < 0) {
        perror("Listen failed");
        close(server_fd);
        exit(1);
    }

    printf("FTP Server running on port %d...\n", PORT);

    // Accept client connection
    client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &addr_len);
    if (client_fd < 0) {
        perror("Accept failed");
        close(server_fd);
        exit(1);
    }

    printf("Client connected!\n");

    // Ask for username
    send(client_fd, "Enter username: ", strlen("Enter username: "), 0);
    memset(buffer, 0, BUFFER_SIZE);
    recv(client_fd, buffer, BUFFER_SIZE, 0);
    buffer[strcspn(buffer, "\n")] = 0; // remove newline
    char username[BUFFER_SIZE];
    strcpy(username, buffer);

    // Ask for password
    send(client_fd, "Enter password: ", strlen("Enter password: "), 0);
    memset(buffer, 0, BUFFER_SIZE);
    recv(client_fd, buffer, BUFFER_SIZE, 0);
    buffer[strcspn(buffer, "\n")] = 0; // remove newline
    char password[BUFFER_SIZE];
    strcpy(password, buffer);

    // Verify credentials
    if (strcmp(username, USERNAME) == 0 && strcmp(password, PASSWORD) == 0) {
        send(client_fd, "Login successful! Welcome to FTP server.\n", 40, 0);
        printf("User '%s' logged in successfully.\n", username);
    } else {
        send(client_fd, "Login failed! Invalid credentials.\n", 35, 0);
        printf("Login attempt failed.\n");
    }

    close(client_fd);
    close(server_fd);
    return 0;
}

