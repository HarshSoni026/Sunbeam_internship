#include<stdio.h>
int main() {
    int result;
    for(result = 0; getchar() != EOF; ++result){
        printf("Current count is: %d\n", result);
        
    }

    return 0;
}