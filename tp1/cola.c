#include "cola.h"
#include <stdlib.h>



//ESTRUCTURAS Y FUNCIONES AUXILIARES NECESARIAS

 typedef struct nodo{
    void* dato;
    struct nodo* prox;
} nodo_t;

nodo_t* nodo_crear(void* valor){
    
    nodo_t* nodo = malloc(sizeof(nodo_t));

    if(nodo == NULL){
        return NULL;
    }

    nodo->prox = NULL;
    nodo->dato = valor;
    return nodo;
}

struct cola{
    nodo_t *prim;
    nodo_t *ult;
};

//PRIMITIVAS DE LA COLA

cola_t* cola_crear(void){
    
    cola_t* cola = malloc(sizeof(cola_t));

    if(cola == NULL){
        return NULL;
    }
    cola->prim = NULL;
    cola->ult = NULL;

    return cola;
}

bool cola_esta_vacia(const cola_t *cola){
    return(cola->prim == NULL && cola->ult == NULL);
}

void* cola_ver_primero(const cola_t *cola){
    
    if(cola_esta_vacia(cola)){
        return NULL;
    }

    return cola->prim->dato;
}

bool cola_encolar(cola_t *cola,void* valor){
    
    nodo_t* nodo_nuevo = nodo_crear(valor);

    if(nodo_nuevo == NULL){
        return false;
    } else if(cola_esta_vacia(cola)){
        cola->prim = nodo_nuevo;
    } else{
        (cola->ult)->prox = nodo_nuevo;
    }
    
    cola->ult = nodo_nuevo;
    return true;
}

void cola_destruir(cola_t *cola, void (*destruir_dato)(void*)){
    
    while(!cola_esta_vacia(cola)){
        void* dato = cola_desencolar(cola);
        
        if(destruir_dato != NULL){
            destruir_dato(dato);
        }
    }

    free(cola);
}

void* cola_desencolar(cola_t *cola){
    
    if(cola_esta_vacia(cola)){
        return NULL;   
    }

    void* valor = (cola->prim)->dato;
    nodo_t* desencolado = cola->prim;

    if((cola->prim)->prox == NULL){
        cola->prim = NULL;
        cola->ult = NULL;
    }
    else{
        nodo_t* nuevo_prim = (cola->prim)->prox;
        cola->prim = nuevo_prim;
    }

    free(desencolado);
    return valor;
}

