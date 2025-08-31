#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

int main() 
{
    int sockfd; 

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0) 
    {
        perror("Socket creation failed");
        exit(1);
    }
    printf("Socket created successfully! sockfd = %d\n", sockfd);
    close(sockfd);

    return 0;
}