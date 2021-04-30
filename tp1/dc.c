#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <stdlib.h>

#include "pila.h"
#include "strutil.h"
#include "cola.h"

#define SUMA "+"
#define RESTA "-"
#define MULTIPLICACION "*"
#define DIVISION "/"
#define RAIZ_CUADRADA "sqrt"
#define EXPONENCIAL "^"
#define LOGARITMO "log"
#define TERNARIO "?"

//Recibe por parametro la pila de operandos, el operador y el puntero al resultado.
//Modifica el valor del resultado dependiendo cuál sea la operación. Devuelve verdadero si se pudo realizar
//la operación correctamente, en caso contrario, devuelve falso.
bool resolver(pila_t* operandos, char* op, long* resultado){
    long a;
    long b;
    long c;
    b = *(long*)pila_desapilar(operandos);

    if(strcmp(op, RAIZ_CUADRADA) == 0){
        if(b < 0){
            return false;
        }

        double res = sqrt((double)b);
        *resultado = (long)res;
        return true;
        }
    
    if(pila_esta_vacia(operandos)){
        return false;
    }

    a = *(long*)pila_desapilar(operandos);
    if(strcmp(op, SUMA) == 0){
        *resultado = a + b;
    }
    if(strcmp(op, RESTA) == 0){
        *resultado = a - b;
    }
    if(strcmp(op, MULTIPLICACION) == 0){
        *resultado = a * b;
    }
    if(strcmp(op, DIVISION) == 0){
        if(b == 0){
            return false;
        }
        *resultado = a / b;
    }
    if(strcmp(op, LOGARITMO) == 0){
        if(b < 2){
            return false;
        }
        double res = log10((double)a) / log10((double)b);
        *resultado = (long)res;
    }
    if(strcmp(op, EXPONENCIAL) == 0){
        if(b < 0){
            return false;
        }
        double res = pow((double)a, (double)b);
        *resultado = (long)res;
        }
    if(strcmp(op, TERNARIO) == 0){
        if(pila_esta_vacia(operandos)){
            return false;
        }

        c = *(long*)pila_desapilar(operandos);
        *resultado = c ? a : b;
    }
    
    return true;
}

//Recibe por parámetro una cadena. Devuelve verdadero si la cadena es un operador, sino, devuelve falso.
bool es_operador(char* str){
    if(strcmp(str, SUMA) == 0 || strcmp(str, MULTIPLICACION) == 0 || strcmp(str, RESTA) == 0 || strcmp(str, RAIZ_CUADRADA) == 0 || strcmp(str, DIVISION) == 0 || strcmp(str, LOGARITMO) == 0 || strcmp(str, EXPONENCIAL) == 0 || strcmp(str, TERNARIO) == 0){
        return true;
    }
    return false;
}

//Recibe por parametro un arreglo dinámico de cadenas, una cola y una pila. Libera la memoria de los mismos.
void liberar_memoria(char** tokens, cola_t* operadores, pila_t* operandos){
    free_strv(tokens);
    cola_destruir(operadores, NULL);
    pila_destruir(operandos);
}

//Recibe por parametro una cadena y devuelve un arreglo dinamico de cadenas de la misma.
char** obtener_operacion(char* linea){
    if(linea[strlen(linea) - 1] == '\n'){
        linea[strlen(linea) - 1] = '\0';
    }
    char** tokens = split(linea, ' ');
    return tokens;
}

//Recibe por parametro una cadena de entrada, un arreglo dinamico de cadenas, una pila, una cola y un puntero a long int.
//(resultado).
//Devuelve verdadero, si los datos de entrada son correctos y se puede resolver la operacion y, además, modifica el valor
//del puntero del resultado. En caso contrario, devuelve falso.
bool obtener_resultado(char* linea, char** tokens, pila_t* operandos, cola_t* aux, long* resultado){
    char* resto = NULL;
    long num;
    size_t j = 0;
    long resultado_aux = 0;
    long numeros[strlen(linea)];

    for(size_t i = 0; tokens[i] != NULL; i++){
        num = strtol(tokens[i], &resto, 10);
        numeros[j] = num;

        if(es_operador(tokens[i])){
            if(pila_esta_vacia(operandos)){
                return false;
            }
            if(!resolver(operandos, tokens[i], &resultado_aux)){
                return false;
            }
            cola_encolar(aux, tokens[i]);
            numeros[j] = resultado_aux;
            pila_apilar(operandos, &numeros[j]);
            j++;
            continue;
        }

        if(strcmp(tokens[i], " ") == 0){
            continue;

        }
        if(num != 0){
            if(resto != tokens[i]){
                pila_apilar(operandos, &numeros[j]);
                j++;
            }
        }
        if(num == 0){
            if(resto != tokens[i]){
                pila_apilar(operandos, &numeros[j]);
                j++;
            }
        }
        if(strlen(resto) > 0){
            return false;
        }
    }

    if((pila_esta_vacia(operandos)) || (!pila_esta_vacia(operandos) && cola_esta_vacia(aux))){
        return false;
    }
    
    *resultado = resultado_aux;
    pila_desapilar(operandos);

    if(!pila_esta_vacia(operandos)){
        return false;
    }

    return true;
}

int main(){
    char* linea = NULL;
    size_t capacidad = 0;
    long resultado = 0;

    while(getline(&linea, &capacidad, stdin) > 0){
        pila_t* operandos = pila_crear();
        cola_t* aux = cola_crear();
        char** tokens = obtener_operacion(linea);

        if(!obtener_resultado(linea, tokens, operandos, aux, &resultado)){
            printf("ERROR\n");
        } else{
             printf("%ld\n", resultado);
        }
        
        liberar_memoria(tokens, aux, operandos);
    }
    
    free(linea);
    return 0;
}

