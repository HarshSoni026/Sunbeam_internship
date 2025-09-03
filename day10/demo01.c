#include<stdio.h>
#include<dirent.h>

int main(void){
    DIR *d;
    struct dirent *dir;

    d = opendir("/home/sunbeam/CDAC/Sunbeam_internship/day10");
    if(d < 0){
        perror("opendir failed....");
    }
    while((dir = readdir(d)) != NULL){
        printf("%s dtype : %d\n", dir->d_name, dir->d_type);
    }
    closedir(d);
    return 0;
}
//see output :
//For directory - 4