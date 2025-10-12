#include<stdio.h>
#include<string.h>

#include<stdlib.h> 

#include<sys/socket.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<netinet/in.h>
#include<dirent.h>

#define PORT 8080
#define BUFFER_SIZE 1024

void send_file(int client_fd, const char *filename){
    FILE *fp = fopen(filename, "rb");
    char buffer[BUFFER_SIZE];
    int n;

    if(!fp){
        perror("File open error");
        return;
    }
    char okmsg[BUFFER_SIZE];
    snprintf(okmsg, sizeof(okmsg), "150 opening data connection for %s\n", filename);
    write(client_fd, okmsg, strlen(okmsg));

    while((n = fread(buffer, 1, sizeof(buffer), fp))>0){
        write(client_fd, buffer, n);

    }
    fclose(fp);

    char *done = "226 transfer complete\n";
    write(client_fd, done, strlen(done));

}

void list_files(int client_fd){
    DIR *d;
    struct dirent *dir;
    char buffer[BUFFER_SIZE];

    d = opendir(".");
        if(!d){
        char *err = "550 Failed to open directory\n";
        write(client_fd, err, strlen(err));
        return;
    }
    write(client_fd, "150 Here comes the directory listing\n", 36);

    while((dir = readdir(d)) != NULL){
        if(strcmp(dir->d_name, ".") == 0 || strcmp(dir->d_name, "..") == 0){
            continue;
        }
        snprintf(buffer, sizeof(buffer), "%s\n", dir->d_name);
        write(client_fd, buffer, strlen(buffer));

    }
    closedir(d);
    write(client_fd, "226 Directory send OK\n", 22);

}

void store_file(int client_fd, const char *filename){
    if(!filename || strlen(filename) == 0){
        char *err = "501 Syntax error in parameters or arguments\n";
        write(client_fd, err, strlen(err));
        return;
    }
    FILE *fp = fopen(filename, "w");
    char buffer[BUFFER_SIZE];
    int n;

    write(client_fd, "150 ready to receive data\n", 26);

    while((n = read(client_fd, buffer, BUFFER_SIZE-1)) > 0){
        buffer[n] = '\0';
        buffer[strcspn(buffer, "\n")] = 0; // Remove newline character

        if(strcmp(buffer, "EOF") == 0){
            break;

        }
        fprintf(fp, "%s\n", buffer);

    }
    fclose(fp);
    write(client_fd, "226 file upload complete\n", 25);
}

void handle_client(int client_fd){
    char buffer[BUFFER_SIZE];
    int n;
    pid_t child_pid = getpid();
    printf("Client handler process ID: %d\n", child_pid);
    
    while((n = read(client_fd, buffer, sizeof(buffer)-1)) > 0){
        buffer[n] = '\0';
        buffer[strcspn(buffer, "\r\n")] = 0;

        //.....QUIT....
        if(strcasecmp(buffer, "quit") == 0){
            char bye_msg[BUFFER_SIZE];
            snprintf(bye_msg, sizeof(bye_msg), "221 Goodbye from server. Handler PID: %d\n", child_pid);
            write(client_fd, bye_msg, strlen(bye_msg));
            break;
        }

        //.....RETR...
        else if(strncasecmp(buffer, "RETR", 4) == 0){
            char *filename = buffer + 5;
            send_file(client_fd, filename);
        }
        //.....STOR....
        else if(strncasecmp(buffer, "STOR", 4) == 0){
            char *filename = buffer + 4;
            while(*filename == ' ') filename++;

            if(*filename == '\0'){
                write(client_fd, "501 Syntax error in parameters or arguments\n", 43);
            }
        }
        //.....LIST....
        else if(strcasecmp(buffer, "LIST") == 0){
            list_files(client_fd);
        }
        else{
            char *err = "502 Command not implemented\n";
            write(client_fd, err, strlen(err));
        }



    }
    printf("Client disconnected, handler PID: %d\n", child_pid);
    close(client_fd);
    exit(0);
}

int main(void){
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    pid_t pid;

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(server_fd < 0){
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }
    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)); 
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);
    
    if(bind(server_fd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0){
        perror("Bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }
    if(listen(server_fd, 5) < 0){
        perror("Listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }
    printf("FTP server listening on port %d\n", PORT);

    while(1){
        client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_addr_len);
        if(client_fd < 0){
            perror("Accept failed");
            continue;
        }
        pid = fork();
        if(pid == 0){
            close(server_fd);

            char welcome_msg[BUFFER_SIZE];
            snprintf(welcome_msg, sizeof(welcome_msg), "220 Welcome to the FTP server. Handler PID: %d\n", getpid());
            write(client_fd, welcome_msg, strlen(welcome_msg));
            handle_client(client_fd);

        }
        else if(pid > 0){
            printf("Spawned child process with PID: %d\n", pid);
            close(client_fd);

        }
        else{
            perror("Fork failed");
            close(client_fd);
        }

    }
    close(server_fd);
    return 0;
}

