
#include <stdio.h>
#include <string.h>

void handle_command(const char *cmd) {
    if (strncmp(cmd, "USER", 4) == 0) {
        printf("USER command received\n");
    } else if (strncmp(cmd, "PASS", 4) == 0) {
        printf("PASS command received\n");
    } else if (strncmp(cmd, "PWD", 3) == 0) {
        printf("Current directory\n");
    } else if (strncmp(cmd, "QUIT", 4) == 0) {
        printf("Goodbye\n");
    } else {
        printf("Unknown command\n");
    }
}

int main() {
    char input[128];
    printf("Enter FTP command: ");
    fgets(input, sizeof(input), stdin);
    handle_command(input);
    return 0;
}
