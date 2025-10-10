//https://robu.in/product/pressure-sensor/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <dirent.h>
#include <netinet/in.h>

#define PORT 2121

int main(void){
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(server_addr);
    char buffer[1024];

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);
    memset(&(server_addr.sin_zero), '\0', 8);
    bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    listen(server_fd, 5);
    printf("FTP server listening on port %d\n", PORT);
    client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &addr_len);
    printf("Client connected\n");
    write(client_fd, "220 Welcome to mini FTP server\r\n", 32);

    while(1){
        int n = read(client_fd, buffer, sizeof(buffer)-1);
        if(n <= 0) break;
        buffer[n] = 0;

        printf("client says > %s", buffer);

        if(strncmp(buffer, "USER ", 5) == 0){
            write(client_fd, "230 login \r\n", 10);

        }
        else if(strncmp(buffer, "STOR", 4) == 0){
            char filename[256];
            sscanf(buffer, "STOR %s", filename);
            FILE *fp = fopen(filename, "w");
            if(!fp){
                write(client_fd, "550 Failed to open file\r\n", 25);
                continue;
            }
            write(client_fd, "150 ready to send data\r\n", 25);
            //read lines until "."
            while((n = read(client_fd, buffer, sizeof(buffer)-1)) > 0){
                buffer[n] = 0;
                if(strcmp(buffer, ".\r\n") == 0) break;
                fputs(buffer, fp);


            }
            fclose(fp);
            write(client_fd, "226 Transfer complete\r\n", 23);
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

