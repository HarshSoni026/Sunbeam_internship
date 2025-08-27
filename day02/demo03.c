#include<stdio.h>
typedef union{
    char var1;
    int var2;
    float var3;
} myUnion;
int main() {
    myUnion u;
    u.var1 = 0x20;
    u.var2 = 0x00;
    printf("Value of var1: %d\n", u.var1);
    printf("Value of var2: %d\n", u.var2);
    return 0;
}