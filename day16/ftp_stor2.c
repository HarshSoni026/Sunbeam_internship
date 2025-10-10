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
    socklen_t addr_len = sizeof(server_addr);
    char buffer[1024];

    //1. create socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(server_fd < 0){
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);
    memset(&(server_addr.sin_zero), 0, sizeof(server_addr.sin_zero));

    //2.bind
    if(bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0){
        perror("bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }
    //3.listen
    if(listen(server_fd, 5) < 0){
        perror("listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }
    printf("FTP server listening on port %d\n", PORT);
    //4.accept
    client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &addr_len);
    if(client_fd < 0){
        perror("accept failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }
    printf("Client connected\n");
    write(client_fd, "220 welcome to my ftp server\r\n", 28);

    while(1){
        int n = read(client_fd, buffer, sizeof(buffer)-1);
        if(n <= 0) break;
        buffer[n] = '\0';
        printf("Received from client: %s", buffer);
        
        if(strncmp(buffer, "USER", 4) == 0){
            write(client_fd, "230 login OK\r\n", 14);

        }
        else if(strncmp(buffer, "STOR", 4) == 0){
            char filename[256];
            sscanf(buffer, "STOR %s", filename);
            FILE *fp = fopen(filename, "w");
            if(!fp){
                write(client_fd, "550 Failed to open file\r\n", 26);
                continue;
            }
            write(client_fd, "150 Opening data connection, ready to receive file, end with single line '.'\r\n", 29);

            //read lines until a single '.' line is received
            while((n = read(client_fd, buffer, sizeof(buffer)-1))>0){
                buffer[n] = 0;
                if(strcmp(buffer, ".\r\n") == 0 || strcmp(buffer, ".\n") == 0){
                    break;

                }
                fputs(buffer, fp);
            }
            fclose(fp);
            write(client_fd, "226 transfer complete\r\n", 20);    
        }
        else if(strncmp(buffer, "QUIT", 4) == 0){
            write(client_fd, "221 Goodbye\r\n", 13);
            break;
        }
        else{
            write(client_fd, "502 Command not implemented\r\n", 29);

        }

    }

    close(client_fd);
    close(server_fd);

    return 0;
}