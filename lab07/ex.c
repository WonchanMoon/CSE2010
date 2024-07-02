#include <stdio.h>

int main(){
    int list[2];
    int* array = list;
    array[0]=1;
    array[1]=2;
    printf("%d\n",array[1]);
}