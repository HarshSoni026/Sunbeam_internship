#include<stdio.h>
#include<dirent.h>

int main(int argc, char *argv[]){

    DIR *d;
    struct dirent *dir;

    d = opendir(argv[1]);
    if(d < 0){
        perror("opendir() failed....");

    }
    while((dir = readdir(d)) != NULL){
        printf("%s\n", dir->d_name);

    }
    closedir(d);
    return 0;
}