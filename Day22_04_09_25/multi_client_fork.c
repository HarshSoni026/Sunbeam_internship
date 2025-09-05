// multi_client_fork.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <dirent.h>

#define PORT 8081
#define BUF_SIZE 1024

void send_file(int client_fd, const char *filename) {
    FILE *fp = fopen(filename, "rb");
    char buffer[BUF_SIZE];
    int n;

    if (!fp) {
        char *err = "550 File not found\r\n";
        write(client_fd, err, strlen(err));
        return;
    }

    char okmsg[BUF_SIZE];
    snprintf(okmsg, sizeof(okmsg), "150 Opening data connection for %s\r\n", filename);
    write(client_fd, okmsg, strlen(okmsg));

    while ((n = fread(buffer, 1, sizeof(buffer), fp)) > 0) {
        write(client_fd, buffer, n);
    }

    fclose(fp);

    char *done = "226 Transfer complete\r\n";
    write(client_fd, done, strlen(done));
}

void list_files(int client_fd) {
    DIR *d;
    struct dirent *dir;
    char buffer[BUF_SIZE];

    d = opendir(".");
    if (!d) {
        char *err = "550 Failed to open directory\r\n";
        write(client_fd, err, strlen(err));
        return;
    }

    write(client_fd, "150 Here comes the directory listing\r\n", 38);

    while ((dir = readdir(d)) != NULL) {
        if (strcmp(dir->d_name, ".") == 0 || strcmp(dir->d_name, "..") == 0)
            continue;
        snprintf(buffer, sizeof(buffer), "%s\r\n", dir->d_name);
        write(client_fd, buffer, strlen(buffer));
    }

    closedir(d);
    write(client_fd, "226 Directory send OK\r\n", 24);
}

void store_file(int client_fd, const char *filename) {
    if (!filename || strlen(filename) == 0) {
        write(client_fd, "501 Syntax error in parameters\r\n", 32);
        return;
    }

    FILE *fp = fopen(filename, "w");
    if (!fp) {
        write(client_fd, "550 Cannot create file\r\n", 25);
        return;
    }

    char buffer[BUF_SIZE];
    int n;

    write(client_fd, "150 Ready to receive data. End with '.' on a line\r\n", 50);

    while ((n = read(client_fd, buffer, BUF_SIZE - 1)) > 0) {
        buffer[n] = '\0';
        buffer[strcspn(buffer, "\r\n")] = 0; // remove CRLF

        if (strcmp(buffer, ".") == 0) {
            break; // End of file data
        }

        fprintf(fp, "%s\n", buffer);
    }

    fclose(fp);
    write(client_fd, "226 File upload complete\r\n", 27);
}

void handle_client(int client_fd) {
    char buffer[BUF_SIZE];
    int n;
    pid_t child_pid = getpid();

    printf("Child process started. PID: %d\n", child_pid);

    while ((n = read(client_fd, buffer, BUF_SIZE - 1)) > 0) {
        buffer[n] = '\0';
        buffer[strcspn(buffer, "\r\n")] = 0;

        // ---- QUIT ----
        if (strcasecmp(buffer, "quit") == 0) {
            char bye_msg[BUF_SIZE];
            snprintf(bye_msg, sizeof(bye_msg), "[PID %d] Goodbye!\n", child_pid);
            write(client_fd, bye_msg, strlen(bye_msg));
            break;
        }

        // ---- RETR filename ----
        else if (strncasecmp(buffer, "RETR ", 5) == 0) {
            char *filename = buffer + 5;
            send_file(client_fd, filename);
        }

        // ---- LIST ----
        else if (strcasecmp(buffer, "LIST") == 0) {
            list_files(client_fd);
        }

        // ---- STOR filename ----
        else if (strncasecmp(buffer, "STOR", 4) == 0) {
            char *filename = buffer + 4;   // point just after "STOR"
            while (*filename == ' ') filename++; // skip spaces

            if (*filename == '\0') {
            // no filename provided
            write(client_fd, "501 Syntax error in parameters\r\n", 32);
    } 
    else {
        store_file(client_fd, filename);
    }
}


        // ---- Unknown Command ----
        else {
            char msg[BUF_SIZE];
            snprintf(msg, sizeof(msg), "500 Unknown command: %s\r\n", buffer);
            write(client_fd, msg, strlen(msg));
        }

        printf("[PID %d] Received: %s\n", child_pid, buffer);
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
        perror("bind"); 
        exit(1);
    }

    listen(server_fd, 5);
    printf("Fork-based server listening on port %d...\n", PORT);

    while (1) {
        client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_len);
        if (client_fd < 0) { perror("accept"); continue; }

        pid_t pid = fork();
        if (pid == 0) {
            close(server_fd);

            char welcome_msg[BUF_SIZE];
            snprintf(welcome_msg, sizeof(welcome_msg), "Welcome! Connected to FTP-like server. [PID %d]\n", getpid());
            write(client_fd, welcome_msg, strlen(welcome_msg));
            handle_client(client_fd);
        }
        else if (pid > 0) {
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
