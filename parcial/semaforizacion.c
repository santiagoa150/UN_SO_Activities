#include <sys/mman.h>
#include <stdio.h>
#include <string.h>
#include <semaphore.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <ctype.h>


char *crear_memoria(int BUFFER_SIZE){    
    /*Función para crear la memoria compartida.*/
    int fd;
    char *ptr;
    fd = shm_open("memoria", O_CREAT | O_RDWR, 0666);
    if (fd == -1){
        perror("Error en shm_open");
        exit(-1);
    }
    
    ftruncate(fd, BUFFER_SIZE);
    ptr = mmap(0, BUFFER_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (ptr == MAP_FAILED){
        perror("Error en MAP_FAILED");
        exit(-1);
    }
    return ptr;
}



int lector(char *ptr, int BUFFER_SIZE, sem_t *father_sem, sem_t *child_sem){
    /* Ejecución del proceso lector.*/
    while (1) {
        char linea[BUFFER_SIZE];
        printf("Ingrese cadena de texto: ");
        fgets(linea, BUFFER_SIZE, stdin);
        linea[strlen(linea) - 1] = '\0'; 
        sprintf(ptr, "%s", linea);
        sem_post(father_sem);
        sem_wait(child_sem);
        printf("Proceso padre recibe de regreso: %s\n", (char *)ptr);
    }
    return 0;
}

void toUpperCase(char str[]){
    /*Función para convertir un string a mayusculas.*/
    int i = 0;
    while (str[i]) {
        str[i] = toupper((unsigned char)str[i]);
        i++;
    }
}


int convertidor(char *ptr, int BUFFER_SIZE, sem_t *father_sem, sem_t *child_sem){
    /* Ejecución del proceso convertidor a mayúsculas.*/
    while(1){
        sem_wait(father_sem);
        char para_convertir[BUFFER_SIZE];
        strcpy(para_convertir, (char *)ptr);
        printf("Proceso hijo recibe: %s\n", para_convertir);
        toUpperCase(para_convertir);
        sprintf(ptr, "%s", para_convertir);
        sem_post(child_sem);
    }
    return 0;
}




int main()
{   
    const int BUFFER_SIZE = 4096;

    /* Se crea la memoria compartida y los semaforos*/
    char *ptr = crear_memoria(BUFFER_SIZE);
        
    sem_t *father_sem;
    sem_t *child_sem;
    father_sem = sem_open("father_sem", O_CREAT, 0666, 0);
    child_sem = sem_open("child_sem", O_CREAT, 0666, 0);
    
    /* Se imprimen los integrantes del parcial.*/
    printf("Parcial 1 - Sistemas operativos (Mecanismos IPC).\n");
    printf("Se utiliza memoria compartida y semaforización.\n");
    printf("Santiago ÁLvarez Muñoz\n");
    printf("Jesús Miguel Porto López\n");
    printf("\n");
    
    int proceso_hijo = fork();
    if (proceso_hijo < 0) {
        printf("Error creando el hijo.");
        exit(-1);
    }
    /*Ejecución del proceso hijo*/ 
    else if (proceso_hijo == 0){
        return convertidor(ptr, BUFFER_SIZE, father_sem, child_sem);
    } 
    /*Ejecución del proceso padre*/
    else {
        return lector(ptr, BUFFER_SIZE, father_sem, child_sem);
    }
    
    /*Se cierra la memoria y los semaforos.*/
    sem_close(father_sem);
    sem_close(child_sem);
    shm_unlink("memoria");
    return 0;
}