#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <unistd.h>
#include <ctype.h>
#include <string.h>

const int SIZE = 4096;
const char *name = "/my_shared_memory";
const char *sem_name = "/my_semaphore";
const char *sem_name2 = "/my_semaphore2";

int main()
{
    printf("\nPARCIAL SISTEMAS OPERATIVOS - MECANISMOS IPC\n");
    printf("Maylen Maria Echavez Morales\n\n");

    // Shared memory is created
    int fd;
    char *ptr;
    fd = shm_open(name, O_CREAT | O_RDWR, 0666);
    if (fd == -1){
        perror("Error en shm_open");
        exit(-1);
    }
    
    ftruncate(fd, SIZE);
    ptr = mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (ptr == MAP_FAILED){
        perror("Error MAP_FAILED");
        exit(-1);
    }

    // Traffic lights are created
    sem_t *father_sem, *child_sem;
    father_sem = sem_open("father_sem", O_CREAT, 0666, 0);
    child_sem = sem_open("child_sem", O_CREAT, 0666, 0);
    
    int rc = fork();
    if (rc > 0) {
        // Father process
        while (1) {
            char text[SIZE];
            printf("Ingrese cadena de texto: ");
            fgets(text, SIZE, stdin);
            text[strcspn(text, "\n")] = '\0'; 
            sprintf(ptr, "%s", text);
            sem_post(father_sem);
            sem_wait(child_sem);
            printf("Proceso padre recibe de regreso: %s\n", (char *)ptr);
        }
    }
    else if (rc == 0) {
        // Child process
        while(1){
            sem_wait(father_sem);
            printf("Proceso hijo recibe: %s\n", (char *)ptr);
            char text2[SIZE];
            strcpy(text2, (char *)ptr);
            for (int i = 0; text2[i]; i++) {
                text2[i] = toupper(text2[i]);
            }
            sprintf(ptr, "%s", text2);
            sem_post(child_sem);
        }
    } else {
        printf("Error creating child process");
        exit(-1);
    }
    shm_unlink(name);
    sem_close(father_sem);
    return 0;
}