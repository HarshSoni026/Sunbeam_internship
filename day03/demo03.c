//Write a program to count blanks, tabs, and newlines.
#include<stdio.h>
int main(){
    int c, nl = 0, nb = 0, nt = 0;
    while((c = getchar()) != EOF){
        if(c == '\n')
            ++nl;
        if(c == ' ')
            ++nb;
        if(c == '\t')
            ++nt;
        printf("Number of lines: %d\n", nl);
        printf("Number of blanks: %d\n", nb);
        printf("Number of tabs: %d\n", nt);    
    }
    
    return 0;
}
