#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<fcntl.h>
#include<netinet/in.h>

#define PORT 2121

int main(void){
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    char buffer[1024] = {0};

    //1.create socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(server_fd < 0){
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    
    }
    printf("Socket created successfully\n");
    //2.bind socket
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if(bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0){
        perror("Bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }
    printf("Socket binded successfully\n");
    //3.listen
    if(listen(server_fd, 5) < 0){
        perror("Listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }
    printf("Server listening on port %d\n", PORT);
    //4.accept
    client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_addr_len);
    if(client_fd < 0){
        perror("Accept failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }
    printf("Client connected\n");
    write(client_fd, "Welcome to my server ,Hello from server\n", 41);



    //5.communicate
    while(1){
        int n = read(client_fd, buffer, sizeof(buffer) - 1);
        if(n <= 0){
            printf("Client disconnected or read error\n");
            break;  
        }
        buffer[n] = '\0';
        printf("Client: %s", buffer); // show the message from client

        if(strncmp(buffer, "USER", 4) == 0){
            write(client_fd, "230 User logged in, proceed\n", 29);
        
        }
        else if(strncmp(buffer, "STOR", 4) == 0){
            char filename[256];
            if(sscanf(buffer, "STOR %s", filename) == 1){
                write(client_fd, "501 syntax error in parameters or arguments\n", 40);
                continue;

            }
            FILE *file = fopen(filename, "wb");
            if(!file){
                write(client_fd, "550 Failed to open file\n", 24);
                continue;   
            }
            write(client_fd, "150 File status okay; about to open data connection\n ready to receive data , end with '.'", 52);

            //read lines until
            while((n = read(client_fd, buffer, sizeof(buffer) - 1)) > 0){
                buffer[n] = '\0';
                if(strcmp(buffer, ".\n") == 0 || strcmp(buffer, ".\r\n") == 0){
                    break; // end of file transfer
                }
                fputs(buffer, file);

            }
            fclose(file);
            write(client_fd, "226 Closing data connection. File transfer successful.\n", 55);
        }
        else if(strncmp(buffer, "QUIT", 4) == 0){
            write(client_fd, "221 Service closing control connection\n", 39);
            break;
        }
        else{
            write(client_fd, "502 Command not implemented\n", 28);
        }   
    }

    close(client_fd);
    close(server_fd);
    return 0;
}