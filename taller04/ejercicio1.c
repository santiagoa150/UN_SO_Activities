#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]){
	printf("Proceso padre (pid:%d)\n", (int) getpid());
	int shared_variable = 0;
	int rc = fork();
	if (rc < 0){
		printf("Falló fork()\n");
		exit(1);
	} else if (rc == 0){
		shared_variable = 1;
		printf("Proceso hijo (pid:%d, shared:%d)\n", (int) getpid(), shared_variable);
	} else{
		wait(NULL);
		printf("Proceso padre (pid:%d, shared:%d)\n", rc, shared_variable);
	}
	return 0;
}
