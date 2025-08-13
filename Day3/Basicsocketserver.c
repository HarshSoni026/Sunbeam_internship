#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>

int main(){
// Create server socket
int server_fd = socket(AF_INET, SOCK_STREAM, 0);
// Assign socket address (IP address + Port no.)
struct sockaddr_in server_addr = {0};
server_addr.sin_family = AF_INET;
server_addr.sin_addr.s_addr = INADDR_ANY;
server_addr.sin_port = htons(2121);

  bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));
  // Listen server socket
    listen(server_fd, 5);
    printf("Echo server running on port 2121\n");
// Accept client connection
    int client_fd = accept(server_fd, NULL, NULL);
    char buffer[1024];
    int bytes;
// read data to client
    while ((bytes = recv(client_fd, buffer, sizeof(buffer), 0)) > 0) {
    //send data to client
        send(client_fd, buffer, bytes, 0);
    }
// close client and server
    close(client_fd);
    close(server_fd);
    return 0;
}

