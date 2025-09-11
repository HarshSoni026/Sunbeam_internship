#include <stdio.h>
#include <string.h>
#include <strings.h>

#define HISTORY_SIZE 5

char history[HISTORY_SIZE][128];
int history_count = 0;

void add_to_history(const char *cmd)
{
    // Shift history if full
    if (history_count == HISTORY_SIZE)
    {
        for (int i = 1; i < HISTORY_SIZE; i++)
        {
            strcpy(history[i - 1], history[i]);
        }
        strcpy(history[HISTORY_SIZE - 1], cmd);
    }
    else
    {
        strcpy(history[history_count++], cmd);
    }
}

void print_history()
{
    printf("\n-- Command History --\n");
    for (int i = 0; i < history_count; i++)
    {
        printf("%d: %s\n", i + 1, history[i]);
    }
    printf("---------------------\n\n");
}

int main()
{
    char input[128];
    char *command, *argument;

    while (1)
    {
        printf("Enter FTP command: ");
        fgets(input, sizeof(input), stdin);
        // Remove trailing newline
        input[strcspn(input, "\n")] = '\0';

        if (strcasecmp(input, "HISTORY") == 0)
        {
            print_history();
            continue;
        }

        if (strcasecmp(input, "QUIT") == 0)
        {
            printf("Goodbye..\n");
            break;
        }

        // Add to history unless it's a control command
        if (strlen(input) > 0)
            add_to_history(input);

        // Parse command and argument
        command = strtok(input, " ");
        printf("COMMAND  : %s\n", command);

        argument = strtok(NULL, " ");
        if (argument && argument[0] != '\0')
            printf("ARGUMENT : %s\n", argument);
        else
            printf("ARGUMENT : <none>\n");
    }

    return 0;
}

