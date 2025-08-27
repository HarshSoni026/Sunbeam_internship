#include<stdio.h>
 

#define MSG "Hello q"

int main(void){
    int a = 98;
    char ch = 'c';

    printf("You entered: %c\t, %d\n", a, ch);

    printf("%10s\n", "India");
    printf("%4s\n", "India");
    printf("%.2s\n", "India");
    printf("%5.2s\n", "India");
    return 0;
}