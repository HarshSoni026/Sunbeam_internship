#include <stdio.h>
#include <dirent.h>

int main(int argc,char *argv[])
{ 
    DIR *d;
    struct dirent *dir;

    d = opendir(argv[1]);
    if(!d)
        perror("opendir");

    while((dir = readdir(d))!= NULL){
        printf("%s\n",dir->d_name);
    }

    closedir(d);

    return 0;
}
