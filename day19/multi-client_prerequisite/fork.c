#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>

int main(){
    int ret;
    ret = fork();
    if(ret == 0){
        printf("child : fork() returned : %d\n", ret);
        printf("child : pid = %d\n", getpid());
        printf("child : ppid = %d\n", getppid());

    }
    else{
        printf("parent : fork() returned : %d\n", ret);
        printf("parent : pid = %d\n", getpid());
        printf("parent : ppid = %d\n", getppid());

    }
    printf("program ends\n");
    getchar();


    return 0;
}