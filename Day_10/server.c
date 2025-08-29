#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<string.h>
#include<unistd.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<dirent.h>

#define PORT    8080
#define IP      "127.0.0.1"

void send_directory_listing(int client_fd) {
    DIR *d;
    struct dirent *dir;
    char file_list[2048] = "";  // Buffer to store file names

    // Task - Read opendir(), readdir(), closedir() from <dirent.h>.
    d = opendir("/home/harsh/0324desd/Internship");  
    if (d) {
        while ((dir = readdir(d)) != NULL) {
            strcat(file_list, dir->d_name);
            strcat(file_list, "\n");
            // Task – Filter only files or only directories (d_type)
            //printf("%s d_type : %d\n", dir->d_name,dir->d_type);  
        }
        closedir(d);
    } else {
        strcpy(file_list, "Failed to open directory.\n");
    }

    write(client_fd, file_list, strlen(file_list));
}

int main()
{
    int server_fd,client_fd;
    int ret;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size;
    char buffer[1024];
    char client_ip[INET_ADDRSTRLEN];

    // 1.Create TCP socket
    server_fd = socket(AF_INET,SOCK_STREAM,0);
    if(server_fd < 0)
    {
        perror("Socket creation failed!");
        exit(0);
    }

    // 2. Set up server address structure
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT); //convert host byte order to network byte order.
    server_addr.sin_addr.s_addr = inet_addr(IP);

    int opt = 1;
    if(setsockopt(server_fd,SOL_SOCKET,SO_REUSEADDR, &opt, sizeof(opt))<0){
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    // 3. Bind the socket to the specified IP and port
    ret = bind(server_fd,(struct sockaddr*)&server_addr,sizeof(server_addr));
    if(ret < 0)
    {
        perror("Bind failed!");
        close(server_fd);
        exit(1);
    }

    listen(server_fd,5);
    printf("Server listening to PORT %d\n",PORT);

    addr_size = sizeof(client_addr);
    client_fd = accept(server_fd,(struct sockaddr*)&client_addr,&addr_size);
    if(client_fd < 0)
    {
        perror("Accept failed!");
        close(server_fd);
        exit(2);
    }

    inet_ntop(AF_INET, &(client_addr.sin_addr), client_ip, INET_ADDRSTRLEN);
    printf("Client connected from %s:%d\n", client_ip, ntohs(client_addr.sin_port));

    char *welcome_msg = "Welcome to the server!\n";
    if(write(client_fd,welcome_msg,strlen(welcome_msg))<0)
    {
        perror("write to client failed!");
    }

    while(1)
    {
        memset(buffer,0,sizeof(buffer));
        int byte_rcv = recv(client_fd,buffer,sizeof(buffer),0);
        if(byte_rcv <= 0)
        {
            printf("Client disconnect or error occurred.\n");
            break;
        }

        buffer[strcspn(buffer, "\r\n")] = 0;

        // Task - Add case-insensitive command matching.
        // Example: user or UsEr should still match USER.
        if(strcasecmp(buffer, "bye") == 0 || strcasecmp(buffer, "exit") == 0)
        {
            printf("Client request to close the connection..!\n");
            break;
        }

        // Task - How directory listing is used in FTP’s LIST command.
        if //(strncmp(buffer, "LIST", 4) == 0)
            (strcasecmp(buffer, "LIST") == 0)  {
            printf("Client requested directory listing.\n");
            send_directory_listing(client_fd);
            continue;
        }

        printf("Client say : %s\n",buffer);
    }

    close(client_fd);
    close(server_fd);

    return 0;
}