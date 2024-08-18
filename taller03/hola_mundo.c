#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[]){
printf("Hola mundo\n");
printf("argc = %d\n", argc);
printf("argv[1] = %s\n", argv[1]);
printf("Conditional %d\n", strcmp(argv[1], "TRUE"));
if(argc > 1 && strcmp(argv[1], "TRUE") != 0){
return 1;
}
return 0;
}
