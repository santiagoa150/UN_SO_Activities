#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

int PAGE_SIZE = 57;
int TOTAL_PAGES = 5;
int VIRTUAL_BITS = 32;
int TLB_SIZE = 57 * 5;

/* Función para solicitar el valor de la dirección de memoria. */
char get_raw_memory_address(char raw_memory_address[230]){

    /* Se solicita la dirección de memoria para manejar. */
    printf("\n");
    printf("Ingrese direccion virtual: ");
    char final_letter[230] = "s\n";
    fgets(raw_memory_address, 230, stdin);

    /* Se verifica si es la instrucción de finalización del sistema. */
    if (strcmp(raw_memory_address, final_letter) == 0){
		printf("Good bye!\n");
		exit(1);
	}
}

/* Función para determinar si hay un Memory Hit. */
bool validate_hit(int *tlb, int memory_address, int *hitPosition){
    for (int page = 0; page < TOTAL_PAGES; page++){

        /* Se verifica si la memoria está guardada en el tlb. */
        if (*tlb == memory_address) {
            *hitPosition = page;
            tlb-=(page * PAGE_SIZE);
            return 1;
        }

        /* Se aumenta la página del tlb.*/
        if (page == 4) tlb -= ((TOTAL_PAGES - 1) * PAGE_SIZE);
        else tlb+=PAGE_SIZE;
    }
    return 0;
}

/* Función para convertir de decimal a binario. */
char *decimal2Binary(int decimal) {
    char *binario = (char *)malloc(VIRTUAL_BITS + 1);
    for (int i = VIRTUAL_BITS - 1; i >= 0; i--) {
        int bit = (decimal >> i) & 1;
        binario[VIRTUAL_BITS - 1 - i] = bit + '0';
    }
    binario[VIRTUAL_BITS] = '\0';
    return binario;
}

/* Función para convertir de binario a decimal. */
int binary2Decimal(char *binary) {
    int decimal = 0;
    int pow = 1; 
    for (int i = strlen(binary) - 1; i >= 0; i--) {
        if (binary[i] == '1') decimal += pow;
        pow *= 2;
    }

    return decimal;
}

/* Función para agregar los datos al tlb. */
void update_tlb_position(
    int *tlb,
    int tlb_position,
    int memory_address,
    int page, 
    int displacement,
    char *binary_displacement,
    char *binary_page
    ){
        tlb += (tlb_position * PAGE_SIZE);
        memcpy(tlb, &memory_address, 4);
        tlb += 4;        
        memcpy(tlb, &page, 4);
        tlb += 4;
        memcpy(tlb, &displacement, 4);
        tlb += 4;
        memcpy(tlb, binary_page, 21);
        tlb += 21;
        memcpy(tlb, binary_displacement, 13);
        tlb += 13;
        tlb -= 46;
        tlb -= (tlb_position * PAGE_SIZE);
}


/* Función para mostrar la dirección y el contenido del TLB. */
void mostrarTLB(int *tlb){
    for (int page = 0; page < TOTAL_PAGES; page++){
        printf("Dirección de memoria %p, contenido: %d\n", tlb, *tlb);
        if (page == 4) tlb -= ((TOTAL_PAGES - 1) * PAGE_SIZE);
        else tlb+=PAGE_SIZE;
    }
}

/* Función que muestra el inicio y el final del TLB. */
void mostrarDireccionesTLB(int *tlb){
    for (int page = 0; page < TOTAL_PAGES; page++){
        tlb+=PAGE_SIZE;
        if (page == 0){
            printf("Inicio del TLB: %p\n", tlb);
        } else if (page == 4){
            printf("Final del TLB: %p\n", tlb);
        }
    }
    tlb -= ((TOTAL_PAGES) * PAGE_SIZE);
}

int main (int argc, char *argv[]){

    /* Se definen los valores iniciales del tlb. */
    int tlb_size  = TLB_SIZE, tlb_position = 0;
    int *tlb = (int *) malloc(tlb_size);

    /* Se imprimen los integrantes del parcial. */
    printf("Parcial 2 - Sistemas operativos (Gestión de memoria).\n");
    printf("Se utiliza TLP y FIFO.\n");
    printf("Santiago Álvarez Muñoz\n");
    printf("Jesús Miguel Porto López\n");

    printf("M: %d\n", 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2);
    printf("N: %d\n", 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2 * 2);
    while (1){
        
        printf("\n");
        mostrarDireccionesTLB(tlb);

        /* Se obtiene la dirección de memoria a procesar. */
        char raw_memory_address[230];
        get_raw_memory_address(raw_memory_address);
        
        /* Se inicializa el tiempo con la función clock_gettime()*/
        struct timespec start, end;
        clock_gettime(CLOCK_MONOTONIC, &start);
        

        /* Se calcula la dirección de memoria en decimal. */
        int memory_address = atoi(raw_memory_address);
        
        /* Se determina si se produce un hit en el tlb. */
        int hit_position;
        bool isHit = validate_hit(tlb, memory_address, &hit_position);

        /* Se inicializan las variables que se deben encontrar. */
        char * binary_memory_address;
        char binary_page[21];
        char binary_displacement[13];
        int page;
        int displacement;
    
        printf("\n");

        /* Caso en que no es un HIT. */
        if (!isHit) {
            /* Se calculan los datos requeridos para el ejercicio cuando es un MISS. */
            binary_memory_address = decimal2Binary(memory_address);
            strncpy(binary_page, binary_memory_address+(0), 20);
            strncpy(binary_displacement, binary_memory_address+(20), 12);
            page = binary2Decimal(binary_page);
            displacement = binary2Decimal(binary_displacement);
        } else {
            /* Se recuperan los datos del caché.*/
            tlb += (hit_position * PAGE_SIZE);
            memory_address = *tlb;
            tlb += 4;
            page = *tlb;
            tlb += 4;
            displacement = *tlb;
            tlb += 4;
            char *page_pointer = (char *)(tlb);
            memcpy(binary_page, page_pointer, 21);
            tlb += 21;  
            char *displacement_pointer = (char *)(tlb);
            memcpy(binary_displacement, displacement_pointer, 13);
            tlb += 13;
            tlb -= 46;
            tlb -= (hit_position  * PAGE_SIZE);
        }

        /* Se actualiza la posición del TLB con FIFO.*/
        update_tlb_position(
            tlb, tlb_position, memory_address, page, displacement,
            binary_displacement, binary_page
        );

        /* Se actualiza la posición del tlb. */
        if (tlb_position < 4) tlb_position ++;
        else tlb_position = 0;
        
        /* Se toma el tiempo final de ejecución. */
        clock_gettime(CLOCK_MONOTONIC, &end);
        double time = (end.tv_sec - start.tv_sec) + (double)(end.tv_nsec - start.tv_nsec) / 1e9;

        /* Se imprimen los datos requiridos. */
        printf("Dirección: %d\n", memory_address);
        printf("TLB %s\n", isHit ? "Hit" : "Miss");
        printf("Página: %d\n", page);
        printf("Desplazamiento: %d\n", displacement);
        printf("Página en binario: %s\n", binary_page);
        printf("Desplazamiento en binario: %s\n", binary_displacement);
        printf("Tiempo: %f segundos\n", time);

        mostrarTLB(tlb);
    }
}