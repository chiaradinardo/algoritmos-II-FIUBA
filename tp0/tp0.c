#include "tp0.h"

/* *****************************************************************
 *                     FUNCIONES A COMPLETAR                       *
 *         (ver en tp0.h la documentación de cada función)         *
 * *****************************************************************/

void swap (int *x, int *y) {
	
	int aux = *x;
	*x = *y;
	*y = aux;
}


int maximo(int vector[], int n) {
	
    if(n == 0){
		return -1;
	}
	
	int posicion_max = 0;
	
	for(int i = 0; i < n; i++){
		if (vector[i] > vector[posicion_max]){
			posicion_max = i;
			}
	}
	return posicion_max;
}


int comparar(int vector1[], int n1, int vector2[], int n2) {
	
	int long_menor;
	if(n1 < n2){
		long_menor = n1;
		}
	if(n1 >= n2){
		long_menor = n2;
		}
	
	if(n1 == 0 && n2 > 0){
		return -1;
		}
	if(n1 > 0 && n2 == 0){
		return 1;
		}
	
	for(int i = 0; i < long_menor; i++){
		if(vector1[i] > vector2[i]){
			return 1;
			}
		if(vector1[i] < vector2[i]){
			return -1;
			}
		}
	
	if(n1 > n2){
		return 1;
		}
	if(n1 < n2){
		return -1;
		}
				
	return 0;	
		
}

void seleccion(int vector[], int n) {
	
	if(n == 0){
		return;
	}
	
	for(int i = n; (i >= 0); i--){
		int pos_max = maximo(vector, i);
		swap(&vector[pos_max], &vector[i - 1]);
	}
}
