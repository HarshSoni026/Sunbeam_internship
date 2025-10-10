#include<stdio.h>
#include<stdint.h>


int main(void){
    int n, num;
    printf("Enter number:\n");
    scanf("%d", &num);
    printf("Enter nth to set:\n");
    scanf("%d", &n);

    num = num | (1<<n);
    printf("%d\n", num);

    return 0;
}