#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

int binarioaDecimal(char *binario) {
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

char *decimalToBinary(int num, int bits) {
    // Asignar memoria para el string binario
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

int main (int argc, char *argv[]) {
    // Se solicita la memoria para el tlb.
    int tlb_size = 5;
    int *tlb = (int *) malloc(sizeof(int) * tlb_size);
    int tlb_position = 0;

    while(1){
        // Se lee la entrada del usuario.
        char input[256];
        printf("\nIngrese direccion virtual: ");
		fgets(input, 256, stdin);
		if (strcmp(input,"s\n") == 0){
			printf("Hasta luego!\n");
		    break;
		}

        // Se calcula el tiempo de inicio.
        struct timespec start, end;
        clock_gettime(CLOCK_MONOTONIC, &start);

        // Se convierte la dirección ingresada a número.
        int direccion, i = 1;
        direccion = atoi(input);

        // Se declaran las variables para imprimir.
        int desplazamiento, pagina;

        while(1){
			if(direccion < 4096 * i) {
			    pagina = i - 1;
			    desplazamiento = direccion - 4096 * pagina;
			    if (desplazamiento == 0){
                    break;
                }
			    break; 
			} else {
			    i++;
			}
		}

        // Se calcula la pagina y el desplazamiento en binario
        char * binaryPage = decimalToBinary(pagina, 20);
        char * binaryMovement = decimalToBinary(desplazamiento, 12);

        bool is_hit = 0;
        for (int i = 0; i < tlb_size; i++) {
            if (tlb[i] == direccion){
                is_hit = 1;
                break;
            }
        }

        tlb[tlb_position] = direccion;

        // Se calcula el delta de ltiempo
        clock_gettime(CLOCK_MONOTONIC, &end);
        long seconds = end.tv_sec - start.tv_sec;
        long nanoseconds = end.tv_nsec - start.tv_nsec;
        double total_seconds = seconds + (double)nanoseconds / 1e9;

        // Se imprimen los datos requeridos.
        printf("Dirección: %d\n", direccion);
        printf("TLB %s\n", is_hit ? "Hit" : "Miss");
        printf("Página: %d\n", pagina);
        printf("Desplazamiento: %d\n", desplazamiento);
        printf("Página en binario: %s\n", binaryPage);
        printf("Desplazamiento en binario: %s\n", binaryMovement);
        printf("Tiempo: %f segundos\n", total_seconds);

        // Se reinicia la posición del TLB.
        if (tlb_position < tlb_size - 1) {
            tlb_position++;
        } else {
            tlb_position = 0;
        }
    }

    // Se libera la memoria para el tlb.
    free(tlb);
	return 0;
}