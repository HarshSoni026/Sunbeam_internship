#include<stdio.h>
#include<string.h>


int main(void){
    char str[] = "this is morning shift of coding with rajat.";
    char *ptr = NULL;
    ptr = str + 10;

    printf("%s\n", str);
    printf("%s\n", ptr);
    ptr = ptr + 3;
    printf("%c\n", *ptr);
    
    return 0;
}