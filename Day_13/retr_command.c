#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main(int argc, char *argv[]) {
    FILE *fp = fopen("file.txt", "rb");
    if (fp == NULL) {
        perror("fopen");
        return 1;
    }

    int client_fd = /* assume already accepted socket fd */;
    char buf[1024];
    int n;

    while ((n = fread(buf, 1, sizeof(buf), fp)) > 0) {
        send(client_fd, buf, n, 0);
    }

    fclose(fp);
    return 0;
}

