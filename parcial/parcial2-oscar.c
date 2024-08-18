#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>

int main(int argc, char *argv[]) {
	printf("\nOscar David Perez Quintero.\n");
	printf("Politica de reemplazo utilizada: FIFO.\nTamano del TLB: 4\n");
	int k = 0;
	int vector[4] = {0,0,0,0};
	int z = 0;
	int p = 0;
	int h = 0;
	bool tb = false;	
	int *tlb = (int *) malloc(sizeof(4));
 int q = 0;
	while(1){
		int numero,pagina;
		char entrada[256];
		char letra[256] = "s\n";
		
		printf("\nIngrese direccion virtual: ");
		fgets(entrada,256,stdin);
		if (strcmp(entrada,letra)==0){
			printf("Hasta luego!\n");
		    	break;
				}
		numero = atoi(entrada);
		if (k==4){
			k=0;
			}
	
		vector[k]=numero;
				int i = 1;

		int desplazamiento;
		k++;
		while(1){
			   if(numero <= 4096*i) {
			   pagina = i-1;
			   desplazamiento = numero-4096*pagina;
				if (desplazamiento==0){break;}
			   printf("Pagina: %d\n",pagina);
			   printf("Desplazamiento: %d\n",desplazamiento);
			   printf("Tiempo de traduccion: \n");
			   break; 
				}
		   else {
			  i++;
			  }
		      }

		
	
		for (z=0;z<4;z++){ 
            //clock_t t1 = clock();
			for(z=0;z<4;z++){
			   //printf("Vector %d = %d\n",z,vector[z]);
				}
					
			if (q==0){
				if (numero == vector[1])
					{
					h++;
					}
				if (numero == vector[2]){
						h++;
						 }
				if (numero== vector[3]) {
						h++;
						 }
				
			if(h==0){
				printf("TLB: Miss\n");
				q++;
				continue;
				}
			else{
				printf("TLB: Hit\n");
				h=0;
				q++;
				continue;
				}
		}

	if (q==1){
				if (numero == vector[2])
					{
					h++;
					}
				if (numero == vector[3]){
						h++;
						 }
				if (numero== vector[0]) {
						h++;
						 }
				
			if(h==0){
				printf("TLB: Miss\n");
				q++;
				continue;
				}
			else{
				printf("TLB: Hit\n");
				h=0;
				q++;
				continue;
				}
		}

if (q==2){
				if (numero == vector[1])
					{
					h++;
					}
				if (numero == vector[0]){
						h++;
						 }
				if (numero== vector[3]) {
						h++;
						 }
				
			if(h==0){
				printf("TLB: Miss\n");
				q++;
				continue;
				}
			else{
				printf("TLB: Hit\n");
				h=0;
				q++;
				continue;
				}
		}

if (q==3){
				if (numero == vector[1])
					{
					h++;
					}
				if (numero == vector[2]){
						h++;
						 }
				if (numero == vector[0]) {
						h++;
						 }
				
			if(h==0){
				printf("TLB: Miss\n");
				q=0;

				}
			else{
				printf("TLB: Hit\n");
				h=0;
				q++;

				}
		}
	if (q>3){q=0;}
//clock_t t2 = clock();
//printf("Tiempo: %d\n",(t1));
	}

	
		}
	free(tlb);
	return 0;
}