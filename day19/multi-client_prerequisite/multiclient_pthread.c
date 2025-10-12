#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h> 

#define PORT 8080
#define BUFFER_SIZE 1024

void *handle_client(void *arg){
    int client_fd = *(int *)arg;
    free(arg);

    char buffer[BUFFER_SIZE];
    int n;

    while((n = read(client_fd, buffer, sizeof(buffer)-1)) > 0){
        buffer[n] = '\0';
        printf("Received: %s", buffer);
        write(client_fd, buffer, strlen(buffer)); // Echo back

    }
    close(client_fd);
    return NULL;

}

int main(void){
    int server_fd, *client_fd;
    struct sockaddr_in server_address, client_address;
    socklen_t client_len = sizeof(client_address);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(server_fd < 0){
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(PORT);
    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    if(bind(server_fd, (struct sockaddr *)&server_address, sizeof(server_address)) < 0){
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
        client_fd = malloc(sizeof(int));
        *client_fd = accept(server_fd, (struct sockaddr *)&client_address, &client_len);
        if(*client_fd < 0){
            perror("Accept failed");
            free(client_fd);
            continue;
        }
        pthread_t tid;
        pthread_create(&tid, NULL, handle_client, client_fd);
        pthread_detach(tid); // Detach thread to free resources on exit
    }

    return 0;
}