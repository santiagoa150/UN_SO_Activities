#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[]){
	int variable = 0;
	pid_t pid;
	pid = fork();
	if (pid < 0){
		printf("Falló fork()\n");
		return 1;
	} else if (pid == 0){
		pid_t pid2;
		pid2 = fork();
		 if (pid2 < 0){
                	printf("Falló fork()\n");
                	return 1;
        	} else if (pid == 0){
                	execlp("/bin/ls", "ls", "-l");
        	} else{
			print("Hijo Trabajando.")
                	wait(NULL);
                	printf("Segundo Hijo termina\n");
	        }
	} else{
		wait(NULL);
		printf("var: %d\n", variable);
		printf("Hijo termina\n");
	}
	return 0;
}

