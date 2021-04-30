#include "lista.h"
#include <stdlib.h>
#include <stddef.h>

typedef struct nodo{
    void* dato;
    struct nodo* prox;
} nodo_t;

struct lista_iter{
    nodo_t* actual;
    nodo_t* ant;
    lista_t* lista;
};

nodo_t* nodo_crearr(void* valor){
    nodo_t* nodo = malloc(sizeof(nodo_t));

    if(!nodo){
        return NULL;
    }

    nodo->prox = NULL;
    nodo->dato = valor;
    return nodo;
}

struct lista{
    size_t largo;
    nodo_t* prim;
    nodo_t* ult;
};

lista_t *lista_crear(void){
    lista_t* lista = malloc(sizeof(lista_t));

    if(!lista){
        return NULL;
    }

    lista->prim = NULL;
    lista->ult = NULL;
    lista->largo = 0;

    return lista;
}

bool lista_esta_vacia(const lista_t *lista){
    return (lista->largo == 0);
}

bool lista_insertar_primero(lista_t *lista, void *dato){
    nodo_t* nuevo_prim = nodo_crearr(dato);

    if(!nuevo_prim){
        return false;
    }
    
    if(!lista_esta_vacia(lista)){
        nuevo_prim->prox = lista->prim;    
    } else{
        lista->ult = nuevo_prim;
    }
    
    lista->prim = nuevo_prim;
    lista->largo++;
    return true;
}

bool lista_insertar_ultimo(lista_t *lista, void *dato){
    nodo_t* nuevo_ult = nodo_crearr(dato);
    
    if(!nuevo_ult){
        return false;
    }

    if(lista_esta_vacia(lista)){
        lista->prim = nuevo_ult;
    } else{
        (lista->ult)->prox = nuevo_ult;
    }
    
    lista->ult = nuevo_ult;
    lista->largo++;
    return true;
}

void *lista_borrar_primero(lista_t *lista){
    if(lista_esta_vacia(lista)){
        return NULL;
    }


    nodo_t* nuevo_prim = (lista->prim)->prox;
    nodo_t* eliminado = lista->prim;
    if(lista->prim == lista->ult){
        lista->ult = nuevo_prim;
    }
    void* dato = (lista->prim)->dato;
    lista->prim = nuevo_prim;
    lista->largo--;

    free(eliminado);
    return dato;
}

void *lista_ver_primero(const lista_t *lista){
    if(lista_esta_vacia(lista)){
        return NULL;
    }
    return (lista->prim)->dato;
}

void *lista_ver_ultimo(const lista_t* lista){
    if(lista_esta_vacia(lista)){
        return NULL;
    }
    return (lista->ult)->dato;
}

size_t lista_largo(const lista_t *lista){
    return lista->largo;
}

void lista_destruir(lista_t *lista, void (*destruir_dato)(void *)){
    while(!lista_esta_vacia(lista)){
        void* dato = lista_borrar_primero(lista);

        if(destruir_dato != NULL){
            destruir_dato(dato);
        }
    }

    free(lista);
}

//PRIMITIVAS DEL ITERADOR EXTERNO

lista_iter_t *lista_iter_crear(lista_t *lista){
    lista_iter_t* iterador = malloc(sizeof(lista_iter_t));
    
    if(!iterador){
        return NULL;
    }

    iterador->actual = lista->prim;
    iterador->ant = NULL;
    iterador->lista = lista;

    return iterador;
}

bool lista_iter_avanzar(lista_iter_t *iter){
    if(!iter || lista_iter_al_final(iter)){
        return false;
    }

    iter->ant = iter->actual;
    iter->actual = (iter->actual)->prox;
    return true;
}

void *lista_iter_ver_actual(const lista_iter_t *iter){
    if(lista_iter_al_final(iter) || lista_esta_vacia(iter->lista)){
        return NULL;
    }
    return (iter->actual)->dato;
}

bool lista_iter_al_final(const lista_iter_t *iter){
    return(!iter->actual);
}

void lista_iter_destruir(lista_iter_t *iter){
    free(iter);
}

bool lista_iter_insertar(lista_iter_t *iter, void *dato){
    nodo_t* nuevo = nodo_crearr(dato);

    if(!iter || !nuevo){
        return false;
    }

    if(!iter->ant && iter->actual){
        (iter->lista)->prim = nuevo;
    } else if(lista_esta_vacia(iter->lista)){
        (iter->lista)->prim = nuevo;
        (iter->lista)->ult = nuevo;
    } else if(lista_iter_al_final(iter)){
        (iter->lista)->ult = nuevo;
        (iter->ant)->prox = nuevo;
    } else if(iter->ant && iter->actual){
        (iter->ant)->prox = nuevo;
    }

    nuevo->prox = iter->actual;
    (iter->lista)->largo++;
    iter->actual = nuevo;
    return true;
}

void *lista_iter_borrar(lista_iter_t *iter){
    if(lista_esta_vacia(iter->lista) || lista_iter_al_final(iter)){
        return NULL;
    }
    if(!iter->ant && iter->actual){
        (iter->lista)->prim = (iter->actual)->prox;        
    }
    if(!(iter->actual)->prox && iter->actual){
        (iter->lista)->ult = iter->ant;
    }
    if(iter->actual && iter->ant){
        (iter->ant)->prox = (iter->actual)->prox;
    }
    
    nodo_t* eliminado = iter->actual;
    void* dato_eliminado = eliminado->dato;
    iter->actual = eliminado->prox;
    free(eliminado);
    (iter->lista)->largo--;
    return dato_eliminado;
}

//PRIMITIVA ITERADOR INTERNO

void lista_iterar(lista_t *lista, bool visitar(void *dato, void *extra), void *extra){
    nodo_t* actual = lista->prim;

    while(actual){
        if(!visitar(actual->dato, extra)){
            break;
        }
        actual = actual->prox;
    }
}