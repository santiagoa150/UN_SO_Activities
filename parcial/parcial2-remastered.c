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

bool validar_tlb(int direccion, int *tlb1, int *tlb2, int *tlb3, int *tlb4, int *tlb5){
    if (
        direccion == *tlb1 || 
        direccion == *tlb2 ||
        direccion == *tlb3 ||
        direccion == *tlb4 ||
        direccion == *tlb5
        ){
            return 1;
        }
    return 0;
}

void save_in_tlb(int tlb_position, int direccion, int *tlb1, int *tlb2, int *tlb3, int *tlb4, int *tlb5){
    if (tlb_position == 0) {
        *tlb1 = direccion;
    } else if (tlb_position == 1){
        *tlb2 = direccion;
    } else if (tlb_position == 2){
        *tlb3 = direccion;
    } else if (tlb_position == 3){
        *tlb4 = direccion;
    } else if (tlb_position == 4){
        *tlb5 = direccion;
    }
}


int main (int argc, char *argv[]) {
    // Se solicita la memoria para el tlb.
    int tlb_size = 5;
    int *tlb1 = (int *) malloc(sizeof(int));
    int *tlb2 = (int *) malloc(sizeof(int));
    int *tlb3 = (int *) malloc(sizeof(int));
    int *tlb4 = (int *) malloc(sizeof(int));
    int *tlb5 = (int *) malloc(sizeof(int));
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

        bool is_hit = validar_tlb(direccion, tlb1, tlb2, tlb3, tlb4, tlb5);
        save_in_tlb(tlb_position,direccion, tlb1, tlb2, tlb3, tlb4, tlb5);


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
    free(tlb1);
    free(tlb2);
    free(tlb3);
    free(tlb4);
    free(tlb5);
	return 0;
}