#define  _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "pila.h"
#include "cola.h"
#include "lista.h"
#include "strutil.h"

void wrapper_free(void* valor){
    free((char*)valor);
}

void obtener_tokens(char* linea, lista_t* lista){
    if(!linea){
        return;
    }

    if(linea[strlen(linea) - 1] == '\n'){
        linea[strlen(linea) - 1] = '\0';
    }
    
    size_t i = 0;
    size_t contador = 0;

    while(linea[i] != '\0'){
        if(isspace(linea[i]) > 0){
            i++;
            continue;
        }
        if(isdigit(linea[i]) == 0){
            lista_insertar_ultimo(lista, substr(&linea[i], 1));
            i++;
            continue;
        }
        while(isdigit(linea[i]) > 0){
            i++;
            contador++;
        }
        lista_insertar_ultimo(lista, substr(&linea[i - contador], contador));
        contador = 0;
    }
}

bool es_numero(char* str){
    char* resto = NULL;
    if(strtol(str, &resto, 10) != 0){
        return true;
    } else if(strtol(str, &resto, 10) == 0){
        if(resto != str){
            return true;
        }
    }
    return false;
}

bool es_operador(char* str){
    if(strcmp(str, "+") == 0 || strcmp(str, "-") == 0 || strcmp(str, "/") == 0 || strcmp(str, "*") == 0 || strcmp(str, "^") == 0){
        return true;
    }
    return false;
}

int precedencia(char* token){
    if(strcmp(token, "^") == 0){
        return 4;
    } else if(strcmp(token, "*") == 0 || strcmp(token, "/") == 0){
        return 3;
    } else{
        return 2;
    }
}

bool es_asociativo_der(char* token){
    if(strcmp(token, "^") == 0){
        return true;
    }
    return false;
}

void convertir(lista_t* tokens, cola_t* salida){
    pila_t* pila_aux = pila_crear();
    lista_iter_t* iter = lista_iter_crear(tokens);
    char* tope;

    while(!lista_iter_al_final(iter)){
        char* actual = (char*)lista_iter_ver_actual(iter);

        if(es_numero(actual)){
            cola_encolar(salida, actual);
        }

        if(es_operador(actual)){
            while(!pila_esta_vacia(pila_aux) && es_operador((char*)pila_ver_tope(pila_aux))){
                tope = (char*)pila_ver_tope(pila_aux);
                if((!es_asociativo_der(actual) && precedencia(actual) <= precedencia(tope)) || (es_asociativo_der(actual) && precedencia(actual) < precedencia(tope))){
                    cola_encolar(salida, pila_desapilar(pila_aux));

                    if(pila_esta_vacia(pila_aux)){
                        break;
                    }

                } else{
                    break;
                }
            }

            pila_apilar(pila_aux, actual);
        }

        if(strcmp(actual, "(") == 0){
            pila_apilar(pila_aux, actual);
        }

        if(strcmp(actual, ")") == 0){
            tope = (char*)pila_ver_tope(pila_aux);
            while(strcmp(tope, "(") != 0){
                cola_encolar(salida, pila_desapilar(pila_aux));
                
                if(pila_esta_vacia(pila_aux)){
                    break;
                }
                tope = (char*)pila_ver_tope(pila_aux);
            }

            pila_desapilar(pila_aux);
        }

        lista_iter_avanzar(iter);
    }

    while(!pila_esta_vacia(pila_aux)){
        cola_encolar(salida, pila_desapilar(pila_aux));
    }

    lista_iter_destruir(iter);
    pila_destruir(pila_aux);
}

int main(){
    char* linea = NULL;
    size_t capacidad = 0;

    while(getline(&linea, &capacidad, stdin) > 0){
        lista_t* tokens = lista_crear();
        cola_t* salida = cola_crear();
        obtener_tokens(linea, tokens);
        convertir(tokens, salida);

        while(!cola_esta_vacia(salida)){
            printf("%s ", (char*)cola_desencolar(salida));
        }
        printf("\n");

        lista_destruir(tokens, wrapper_free);
        cola_destruir(salida, NULL);
    }
    
    free(linea);
    return 0;
}



