#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/types.h>
#include<unistd.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<stdlib.h>

#define PORT 2121

int main(void){
    int socket_fd, client_fd;
    struct sockaddr_in server_addr;
    socklen_t addr_len = sizeof(server_addr);
    char buffer[1024];

    socket_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(socket_fd < 0){
        perror("Socket creation failed");
        exit(EXIT_FAILURE); 
    }
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    bind(socket_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));
    listen(socket_fd, 5);
    printf("Server listening on port %d\n", PORT);
    client_fd = accept(socket_fd, (struct sockaddr*)&server_addr, &addr_len);
    if(client_fd < 0){
        perror("Accept failed");
        close(socket_fd);
        exit(EXIT_FAILURE);
    }
    printf("Client connected\n");   
    write(client_fd, "220 Welcome to the FTP server\r\n", 31);
    while(1){
        int n = read(client_fd, buffer, sizeof(buffer)-1);
        if(n <= 0) break;
        buffer[n] = '\0';
        printf("Received: %s", buffer); //show received command
        if(strncmp(buffer, "USER", 4) == 0){
            write(client_fd, "230 User logged in\r\n", 20);

        }
        //task 4 - append command
        else if(strncmp(buffer, "APPEND", 6) == 0){
            char filename[256];
            if(sscanf(buffer, "APPEND %s", filename) != 1){
                write(client_fd, "501 syntax error in parameters or arguments\r\n", 42);
                continue;

            }
            FILE *file = fopen(filename, "a");
            if(!file){
                write(client_fd, "550 File not found or access denied\r\n", 38);
                continue;
            }
            write(client_fd, "150 file status okay; about to open data connection\r\n", 49);

            //read lines until "."
            while((n = read(client_fd, buffer, sizeof(buffer)-1)) > 0){
                buffer[n] = '\0';
                if(strcmp(buffer, ".\r\n") == 0) break;
                fputs(buffer, file);


            }
            fclose(file);
            write(client_fd, "226 Closing data connection. File transfer successful\r\n", 54);

        }
        else if(strncmp(buffer, "QUIT", 4) == 0){
            write(client_fd, "221 Service closing control connection\r\n", 39);
            break;
        }
        else if(strncmp(buffer, "RANDOMCMD", 9) == 0){
            write(client_fd, "550 Unknown command\r\n", 20);
        }
        else{
            write(client_fd, "502 Command not implemented\r\n", 29);
        }
    }
    close(client_fd);
    close(socket_fd);
    return 0;
}