#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

void handle_client(int client_fd){
    char buffer[BUFFER_SIZE];
    int n;
    pid_t child_pid = getpid();

    printf("Child process %d handling client\n", child_pid);

    while((n = read(client_fd, buffer, BUFFER_SIZE-1))>0){
        buffer[n] = '\0';
        buffer[strcspn(buffer, "\r\n")] = 0;

        //check for quit command
        if(strcasecmp(buffer, "quit") == 0){
            char bye_msg[BUFFER_SIZE];
            snprintf(bye_msg, sizeof(bye_msg), "[PID %d] GOODBYE\n", child_pid);
            write(client_fd, bye_msg, strlen(bye_msg));
            break;

        }
        printf("[PID %d] Received: %s\n", child_pid, buffer);
        char reply[BUFFER_SIZE];
        snprintf(reply, sizeof(reply), "[PID %d] Echo : %s\n", child_pid, buffer);
        write(client_fd, reply, strlen(reply));
    }

    printf("[PID %d] client disconnected\n", child_pid);
    close(client_fd);
    exit(0);
}

int main(void){
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(server_fd < 0){
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);
    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    if(bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0){
        perror("Bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }
    if(listen(server_fd, 5) < 0){
        perror("Listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }
    printf("Server listening on port %d\n", PORT);

    while(1){
        client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_len);

        pid_t pid = fork();
        if(pid == 0){
            perror("Fork failed");
            close(server_fd);   
            handle_client(client_fd);
        }
        else if(pid > 0){
            //parent process
            printf("spawned child process %d for new client\n", pid);
            close(client_fd);

        }
        else{
            perror("fork failed");
            close(client_fd);
        }


    }
    close(server_fd);
    
    return 0;
}