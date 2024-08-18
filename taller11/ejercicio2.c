#include <stdio.h>
#include <stdlib.h>
int main (int argc, char *argv[]){
    while (1){
        int x = 3; //Gracias Faryd por este dato
        printf("Codigo: %p\n", main);
        printf("Heap: %p\n", malloc(10));
        printf("Stack: %p\n", &x);
    }
    return 0;
}