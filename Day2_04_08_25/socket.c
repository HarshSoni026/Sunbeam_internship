#include<stdio.h>       // For prinf(), perror()
#include<stdlib.h>      // For exit()
#include<sys/socket.h>  // For socket(), bind(), listen(), accept() functions and socket types (SOCK_STREAM etc.)
#include<netinet/in.h>  // For sockaddr_in structure, AF_INET, htons(), INADDR_ANY
#include<unistd.h>      // For close

int main()
{
    int socket_fd;
    
    // Creating Socket
    socket_fd = socket(AF_INET,SOCK_STREAM,0);
    if(socket_fd < 0)
    {
        perror("socket creation failed!");
        exit(0);
    }
    printf("socket created successfully... socket_fd = %d\n",socket_fd);

    close(socket_fd);

    return 0;
}