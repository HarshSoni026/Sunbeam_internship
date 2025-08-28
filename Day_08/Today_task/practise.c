#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<dirent.h>

#define PORT 8081
#define IP "127.0.0.1"

void send_directory_listing(int client_fd)
{
    DIR *d;
    struct dirent *dir;
    char file_list[1024] = "";
    d = opendir(".");
    if(!d){
        perror("opendir");
    }

    while
    (
        (dir = readdir(d))!= NULL){
            strcat(file_list,dir->d_name);
            strcat(file_list,"\n");
    }
    closedir(d);
    
    write(client_fd,file_list,strlen(file_list));
    
}


int main()
{
    int server_fd, client_fd,ret;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size;

    char buffer[1024];
    char client_ip[INET_ADDRSTRLEN];

    server_fd = socket(AF_INET,SOCK_STREAM,0);
    if(server_fd<0)
    {
        perror("socket creation failed!\n");
        exit(0);
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr(IP);

    ret = bind(server_fd,(struct sockaddr*)&server_addr,sizeof(server_addr));
    if(ret < 0)
    {
        perror("Bind failed!\n");
        exit(1);
    }

    if(listen(server_fd, 5) < 0) {
        perror("listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    addr_size = sizeof(client_addr);
    client_fd = accept(server_fd,(struct sockaddr*)&client_addr,&addr_size);

    inet_ntop(AF_INET,&client_addr.sin_addr,client_ip,sizeof(client_ip));
    printf("connected to client : %s %d\n",client_ip,ntohs(client_addr.sin_port));

    char *Welcome_message = "Welcome to server\n";
    if(write(client_fd,Welcome_message,sizeof(Welcome_message))<0){
        perror("write to client failed!\n");
    }

    while(1)
    {
        memset(buffer,0,sizeof(buffer));
        int byte_receive = recv(client_fd,buffer,sizeof(buffer),0);
        if(byte_receive <= 0)
        {
            perror("Client disconnect or error occured!\n");
            break;
        }

        buffer[strcspn(buffer,"\r\n")]==0;

        if(strncmp(buffer,"exit",4)==0){
            printf("client requst to disconnect..\n");
            break;
        }

        if(strncmp(buffer,"LIST",4)==0){
            printf("Client requested directory listing.\n");
            send_directory_listing(client_fd);
            continue;
        }
    }

    close(client_fd);
    close(server_fd);

    return 0;
} 