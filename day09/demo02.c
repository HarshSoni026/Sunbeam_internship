#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<netinet/ip.h>
#include<dirent.h>


#define PORT 8081
#define IP "127.0.0.1"


void send_directory_list(int client_fd){
    DIR *d;
    struct dirent *dir;
    char file_list[1024];

    d = opendir("/home/sunbeam/CDAC/Sunbeam_internship");
    if(d){
        while((dir = readdir(d)) != NULL){
            strcat(file_list, dir->d_name);
            strcat(file_list, "\n");
        }
        closedir(d);
    }
    else{
        strcpy(file_list, "Failed to open directory");
    }
    write(client_fd, file_list, strlen(file_list));

}

int main(void){
    int server_fd, client_fd, ret;

    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size;

    char buffer[1024];
    char client_ip[INET_ADDRSTRLEN];

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr(IP);


    int opt = 1;
    if(setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0){
        perror("setsockopt failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }
    ret = bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));
    if(ret < 0){
        perror("bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    listen(server_fd, 5);
    printf("Server listening to port number : %d", PORT);

    addr_size = sizeof(client_addr);

    client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &addr_size);

   

    inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, sizeof(client_ip));
    printf("connected to client ip: %s:%d\n", client_ip, ntohs(client_addr.sin_port));

    char *welcome_msg = "Welcome to the server\n";
    write(client_fd, welcome_msg, strlen(welcome_msg));

    while(1){
        memset(buffer, 0, sizeof(buffer));
        int bytes_received = recv(client_fd, buffer, sizeof(buffer), 0);
        if(bytes_received <= 0){
            printf("Client disconnected\n");
            break;
        }

        buffer[strcspn(buffer, "\r\n")] = 0;

        if(strncmp(buffer, "exit", 4) == 0){
            printf("client requested to close the connection.\n");
            break;
        }

        if(strncmp(buffer, "LIST", 4) == 0){
            printf("Client requested directory listing.\n");
            send_directory_list(client_fd);
            continue;
        }
        
        printf("Client says : %s\n", buffer);
    }
    close(client_fd);
    close(server_fd);
    

    return 0;
}