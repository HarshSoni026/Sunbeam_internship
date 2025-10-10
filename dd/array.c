#include<stdio.h>
int main(){
    int arr[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    int *ptr = &arr;
    for(int i = 9; i>=0;i--){
        arr[i] = ptr+1;
    }

    for(int j = 0; j < 10; j++){
        printf("%d\n", *ptr);
    }

    return 0;
}