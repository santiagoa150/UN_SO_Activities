#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

char *decimalABinario(int num) {
    // Asignar memoria para el string binario
    int bits = 32;
    char *binario = (char *)malloc(bits + 1);
    if (binario == NULL) {
        printf("Error de asignación de memoria.\n");
        exit(EXIT_FAILURE);
    }

    // Iterar a través de cada bit
    for (int i = bits - 1; i >= 0; i--) {
        // Usar el operador de desplazamiento a la derecha y el operador AND
        int bit = (num >> i) & 1;
        binario[bits - 1 - i] = bit + '0';
    }

    // Agregar el carácter nulo al final del string
    binario[bits] = '\0';
    return binario;
}

int binarioADecimal(char *binario) {
    // Inicializar decimal y potencia
    int decimal = 0;
    int potencia = 1; 

    // Iterar a través de la cadena binaria de derecha a izquierda
    for (int i = strlen(binario) - 1; i >= 0; i--) {
        if (binario[i] == '1') {
            decimal += potencia;
        }

        // Multiplicar la potencia por 2 para el siguiente bit
        potencia = potencia * 2;
    }

    return decimal;
}


int main (int argc, char *argv[]){
    
    printf("\nPARCIAL 2 SISTEMAS OPERATIVOS\n");
    printf("Maylen Maria Echavez Morales\n\n");

    
    int tamano_tlb  = 285;
    int *tlb = (int *) malloc(tamano_tlb);
    int posicion_tlb = 0;
    
    while (1) {
        char input[100];
        printf("\nIngrese direccion virtual: ");
		fgets(input, 100, stdin);
		if (strcmp(input,"s\n") == 0){
			printf("Good bye!\n");
		    break;
		}

        // Se calcula el tiempo de inicio.
        struct timespec inicio, final;
        clock_gettime(CLOCK_MONOTONIC, &inicio);
        
        // Se convierte a número la dirección.
        int direccion = atoi(input);
        bool es_hit = 0;
        int posicion_hit = 0;
        int sumed = 0;
        for (int i = 0; i < 5; i++){
            if (*tlb == direccion) {
                es_hit = 1;
                posicion_hit = i;
                tlb-=sumed;
                break;
            }
            if (i == 4){
                tlb -= 228;
            } else {
                tlb+=57;
                sumed+=57;
            }
        }
        
        printf("\n");

        char * direccion_binaria;
        char pagina_binaria[21];
        char desplazamiento_binario[13];
        int pagina;
        int desplazamiento;

        if (es_hit){
            tlb += (posicion_hit * 57);
            printf("Dirección: %d\n", *tlb);
            direccion = *tlb;
            printf("TLB Hit\n");
            tlb += 4;
            printf("Página: %d\n", *tlb);
            pagina = *tlb;
            tlb += 4;
            printf("Desplazamiento: %d\n", *tlb);
            desplazamiento = *tlb;
            tlb += 4;
            char *ptr_pagina = (char *)(tlb);
            printf("Página en binario: %s\n", ptr_pagina);
            memcpy(pagina_binaria, ptr_pagina, sizeof(ptr_pagina));
            tlb += 21;
            char *ptr_desplazamiento = (char *)(tlb);
            printf("Desplazamiento en binario: %s\n", ptr_desplazamiento);
            memcpy(desplazamiento_binario, ptr_desplazamiento, sizeof(ptr_desplazamiento));
            tlb += 13;
            tlb -= 46;
            tlb -= (posicion_hit * 57);
        } else {
            direccion_binaria = decimalABinario(direccion);




            strncpy(pagina_binaria, direccion_binaria+(1-1), 20);
            strncpy(desplazamiento_binario, direccion_binaria+(21-1), 12);

            pagina = binarioADecimal(pagina_binaria);
            desplazamiento = binarioADecimal(desplazamiento_binario);

            printf("Dirección: %d\n", direccion);
            printf("TLB Miss\n");
            printf("Página: %d\n", pagina);
            printf("Desplazamiento: %d\n", desplazamiento);
            printf("Página en binario: %s\n", pagina_binaria);
            printf("Desplazamiento en binario: %s\n", desplazamiento_binario);
        }
        
        tlb += (posicion_tlb * 57);
        memcpy(tlb, &direccion, 4);
        tlb += 4;
        memcpy(tlb, &pagina, 4);
        tlb += 4;
        memcpy(tlb, &desplazamiento, 4);
        tlb += 4;
        memcpy(tlb, &pagina_binaria, 21);
        tlb += 21;
        memcpy(tlb, &desplazamiento_binario, 13);
        tlb += 13;
        tlb -= 46;
        tlb -= (posicion_tlb * 57);

        if (posicion_tlb == 4){
                posicion_tlb = 0;
        } else {
                posicion_tlb++;
        }

        // Se calcula el delta de ltiempo
        clock_gettime(CLOCK_MONOTONIC, &final);
        long segundos = final.tv_sec - inicio.tv_sec;
        long nanosegundos = final.tv_nsec - inicio.tv_nsec;
        double total_segundos = segundos + (double)nanosegundos / 1e9;
        printf("Tiempo: %f segundos\n", total_segundos);
    }

    free(tlb);
}