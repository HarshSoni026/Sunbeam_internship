// - Try uploading two files in the same session:
//    STOR file1.txt   (type some text, then '.')
//    STOR file2.txt  (type some text, then '.')
// - Verify both files are created.



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 2121

int main() {
    int server_fd, client_fd;
    struct sockaddr_in addr;
    socklen_t addrlen = sizeof(addr);
    char buffer[1024];

    // 1. Create socket
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(PORT);

    bind(server_fd, (struct sockaddr *)&addr, sizeof(addr));
    listen(server_fd, 1);

    printf("📡 FTP STOR demo server running on port %d...\n", PORT);

    client_fd = accept(server_fd, (struct sockaddr *)&addr, &addrlen);
    write(client_fd, "220 Simple FTP Server Ready\r\n", 30);

    while (1) {
        int n = read(client_fd, buffer, sizeof(buffer) - 1);
        if (n <= 0) break;
        buffer[n] = 0;

        printf("Client> %s", buffer); 

        if (strncmp(buffer, "USER", 4) == 0) {
            write(client_fd, "230 Login OK\r\n", 14);
        } 
        else if (strncmp(buffer, "STOR", 4) == 0) {
            char filename[100];
            sscanf(buffer, "STOR %s", filename);

            FILE *fp = fopen(filename, "w");
            if (!fp) {
                write(client_fd, "550 Cannot open file\r\n", 22);
                continue;
            }

            write(client_fd, "150 Ready to receive data, end with a single '.' line\r\n", 54);

            // Read lines until "."
            while ((n = read(client_fd, buffer, sizeof(buffer) - 1)) > 0) {
                buffer[n] = '\0';

                // end marker
                if (strcmp(buffer, ".\r\n") == 0 || strcmp(buffer, ".\n") == 0) {
                    break;
                }

                fputs(buffer, fp);
            }

            fclose(fp);
            write(client_fd, "226 Transfer complete\r\n", 23);
        } 
        else if (strncmp(buffer, "QUIT", 4) == 0) {
            write(client_fd, "221 Goodbye\r\n", 13);
            break;
        } 
        else {
            write(client_fd, "502 Command not implemented\r\n", 30);
        }
    }

    close(client_fd);
    close(server_fd);
    return 0;
}