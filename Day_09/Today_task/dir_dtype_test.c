#include <stdio.h>
#include <dirent.h>

int main() {
    DIR *d;
    struct dirent *dir;

    d = opendir("/home/harsh/0324desd/Internship");
    if(!d)
        perror("opendir");
    
    while ((dir = readdir(d)) != NULL) {
        // Task1 : Use dir->d_type
        printf("%s d_type : %d\n", dir->d_name,dir->d_type);  // Diretory - o/p 4
    }                                                         // Regular file - o/p 8
    closedir(d);
            
    return 0;
}
