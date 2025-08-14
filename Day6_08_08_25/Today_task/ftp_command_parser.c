
#include <stdio.h>
#include <strings.h>
#include <string.h>

void handle_command(const char *cmd) {
    if (strcasecmp(cmd, "USER") == 0) {
        printf("USER command received\n");
    } else if (strcasecmp(cmd, "PASS") == 0) {
        printf("PASS command received\n");
    } else if (strcasecmp(cmd, "PWD") == 0) {
        printf("Current directory\n");
    } else if (strcasecmp(cmd, "QUIT") == 0) {
        printf("Goodbye\n");
    } else {
        printf("Unknown command\n");
    }
}

int main() {
    char input[128];
    printf("Enter FTP command: ");
    fgets(input, sizeof(input), stdin);
    //cspn meaning in c This line removes the trailing newline from a string read by fgets.
    input[strcspn(input, "\n")] = 0; 
    handle_command(input);
    return 0;
}
