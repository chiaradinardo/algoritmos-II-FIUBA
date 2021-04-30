#ifndef LISTA_H
#define LISTA_H

#include <stdbool.h>
#include <stddef.h>

//DEFINICIÓN DE TIPO DE DATOS

struct lista;
typedef struct lista lista_t;

//DEFINICIÓN DEL ITERADOR EXTERNO
typedef struct lista_iter lista_iter_t;

//PRIMITIVAS DE LA LISTA

//Crea una lista.
//Post: devuelve una nueva lista vacía.
lista_t *lista_crear(void);

//Devuelve verdadero si la lista está vacía y en caso contrario, falso.
//Pre: la lista fue creada.
bool lista_esta_vacia(const lista_t *lista);

//Inserta un elemento cualquiera el principio de la lista.
//Pre: la lista fue creada.
//Post: la lista tiene un elemento nuevo al comienzo de la lista.
bool lista_insertar_primero(lista_t *lista, void *dato);

//Inserta un elemento cualquiera al final de la lista.
//Pre: la lista fue creada.
//Post: la lista tiene un elemento nuevo al final del principio.
bool lista_insertar_ultimo(lista_t *lista, void *dato);

//Elimina el primer elemento de lista y devuelve el valor del mismo.
//Pre: la lista fue creada.
//Post: la lista tiene un elemento menos en la lista y se devolvió el valor de ese elemento.
void *lista_borrar_primero(lista_t *lista);

//Devuelve el valor del primer elemento de la lista.
//Pre: la lista fue creada.
//Post: se devolvió el primer elemento de la lista si no está vacía, y si lo está se devuelve NULL.
void *lista_ver_primero(const lista_t *lista);

//Devuelve el valor del último elemento de la lista.
//Pre: la lista fue creada.
//Post: se devolvió el último elemento de la lista si no está vacía, y si lo está se devuelve NULL.
void *lista_ver_ultimo(const lista_t* lista);

//Devuelve el largo de la lista.
//Pre: la lista fue creada.
//Post: se devolvió el tamaño de la lista.
size_t lista_largo(const lista_t *lista);

// Destruye la lista. Si se recibe la función destruir_dato por parámetro,
// para cada uno de los elementos de la lista llama a destruir_dato.
// Pre: la lista fue creada. destruir_dato es una función capaz de destruir
// los datos de la cola, o NULL en caso de que no se la utilice.
// Post: se eliminaron todos los elementos de la lista.
void lista_destruir(lista_t *lista, void (*destruir_dato)(void *));

//Recorre la lista y se le aplica la función visitar a casa elemento de la misma.
//Pre: la lista fue creada.
//Post: los elementos de la lista cambiaron dependiendo la función visitar.
void lista_iterar(lista_t *lista, bool visitar(void *dato, void *extra), void *extra);

//Crea un iterador externo para recorrer la lista.
//Pre: la lista fue creada.
lista_iter_t *lista_iter_crear(lista_t *lista);

//Avanza al siguiente elemento respecto a donde se encuentre ubicado el iterador.
//Pre: el iterador fue creado.
//Post: el iterador se encuentra en una posición siguiente a donde estaba inicialmente.
bool lista_iter_avanzar(lista_iter_t *iter);

//Devuelve el dato de donde se encuentra el iterador.
//Pre: el iterador fue creado.
void *lista_iter_ver_actual(const lista_iter_t *iter);

//Devuelve verdadero si el iterador se encuentra en el último elemento de la lista, en caso contrario,
//devuelve falso.
//Pre: el iterador fue creado.
bool lista_iter_al_final(const lista_iter_t *iter);

//Destruye el iterador.
//Pre: el iterador fue creado.
//Post: el iterador se destruyó.
void lista_iter_destruir(lista_iter_t *iter);

//Inserta un elemento en el lugar donde el iterador esté ubicado.
//Pre: la lista y el iterador fueron creados.
//Post: la lista tiene un nuevo elemento.
bool lista_iter_insertar(lista_iter_t *iter, void *dato);

//Elimina el elemento en donde se encuentra posicionado el iterador.
//Pre: la lista y el iterador fueron creados.
//Post: la lista tiene un elemento menos en ella.
void *lista_iter_borrar(lista_iter_t *iter);

#endif