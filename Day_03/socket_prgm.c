// Header for standard I/O functions like printf(), perror()
#include<stdio.h>

// Header for general utilities like exit()
#include<stdlib.h>

// Header for string operations like memset(), strlen()
#include<string.h>

// Header for socket functions like socket(), bind(), listen(), accept(), recv(), send()
#include<sys/socket.h>

// Header for POSIX system calls like close()
#include<unistd.h>

// Header for Internet address structures like sockaddr_in, INADDR_ANY
#include<netinet/in.h>

// Header for converting IP addresses (e.g., inet_ntop())
#include<arpa/inet.h>

#define PORT    8080
#define IP    "127.0.0.1"

int main()
{
    int server_fd, client_fd;                       // Socket file descriptors
    int ret;                                        
    struct sockaddr_in server_addr, client_addr;    // Server and client address structures
    socklen_t addr_size;                            // Size of the client address structure
    char buffer[1024];                              // Buffer to hold incoming messages
    char client_ip[INET_ADDRSTRLEN];                // To store client IP address in string format

    // Step 1. Create a TCP socket using IPv4
    // Parameters: AF_INET = IPv4, SOCK_STREAM = TCP, 0 = default protocol
    server_fd = socket(AF_INET,SOCK_STREAM,0);
    if (server_fd < 0) {
        perror("Socket creation failed");
        exit(1);
    }

    // Step 2. Setup server address
    server_addr.sin_family = AF_INET;                // Use IPv4
    server_addr.sin_port = htons(PORT);              // Port 8080 (convert to network byte order)
    server_addr.sin_addr.s_addr = inet_addr(IP);     // Accept connections on "127.0.0.1"

    // Step 3. Bind the socket to the specified IP and port
    ret = bind(server_fd,(struct sockaddr*)&server_addr,sizeof(server_addr));
    if(ret < 0)
    {
        perror("Bind Failed!");
        close(server_fd);
        exit(1);
    }
    printf("Bind successfully...\n");

    // Step 4. Start listening for incoming connections
    // The second argument is the backlog size (queue limit)
    listen(server_fd,5);
    printf("server listning on PORT 8080...\n");

     // Step 5. Accept an incoming client connection
    addr_size = sizeof(client_addr);
    client_fd = accept(server_fd,(struct sockaddr*)&client_addr,&addr_size);
    if (client_fd < 0) {
        perror("Accept failed");
        close(server_fd);
        exit(1);
    }
    
    // Receive message from the client
    recv(client_fd,buffer,sizeof(buffer),0);
    printf("Client msg: %s\n",buffer);

    // Send a response back to the client
    char *reply = "welcome from server!"; 
    send(client_fd, reply, strlen(reply),0);

    // Step 6. Close both sockets
    close(client_fd);
    close(server_fd);

    return 0;
}