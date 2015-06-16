#ifndef LISTA_H
#define LISTA_H

#include <stdbool.h>
#include <stddef.h>

/* ******************************************************************
 *                DEFINICION DE LOS TIPOS DE DATOS
 * *****************************************************************/

typedef struct lista lista_t;

typedef struct lista_iter lista_iter_t;

/* ******************************************************************
 *                 PRIMITIVAS  BASICAS DE LA LISTA
 * *****************************************************************/

// Crea una lista.
// Post: devuelve una nueva lista vacía.
lista_t *lista_crear();

// Devuelve verdadero o falso, según si la lista tiene o no 
//elementos agregados.
// Pre: la lista fue creada.
bool lista_esta_vacia(const lista_t *lista);

// Agrega un nuevo elemento al principio de la lista. Devuelve falso 
// en caso de error.
// Pre: la lista fue creada.
// Post: se agregó un nuevo elemento a la lista, dato se encuentra al inicio
// de la lista.
bool lista_insertar_primero(lista_t *lista, void *dato);

// Agrega un nuevo elemento al final de la lista. Devuelve falso 
//en caso de error.
// Pre: la lista fue creada.
// Post: se agregó un nuevo elemento a la lista, dato se encuentra al final
// de la lista.
bool lista_insertar_ultimo(lista_t *lista, void *dato);

// Saca el primer elemento de la lista. Si la lista tiene elementos,
// se quita el primero de la lista, y se devuelve su valor, si está 
// vacía, devuelve NULL.
// Pre: la lista fue creada.
// Post: se devolvió el valor del primer elemento anterior, la lista
// contiene un elemento menos, si la lista no estaba vacía.
void *lista_borrar_primero(lista_t *lista);

// Obtiene el valor del primer elemento de la lista. Si la lista tiene
// elementos, se devuelve el valor del primero, si está vacía devuelve NULL.
// Pre: la lista fue creada.
// Post: se devolvió el primer elemento de la lista, cuando no está vacía.
void *lista_ver_primero(const lista_t *lista); 

// Devuelve el valor del largo de la lista.
// Pre: la lista fue creada.
size_t lista_largo(const lista_t *lista);

// Destruye la lista. Si se recibe la función destruir_dato por parámetro,
// para cada uno de los elementos de la lista llama a destruir_dato.
// Pre: la lista fue creada. destruir_dato es una función capaz de destruir
// los datos de la lista, o NULL en caso de que no se la utilice.
// Post: se eliminaron todos los elementos de la lista.
void lista_destruir(lista_t *lista, void destruir_dato(void *));

/* ******************************************************************
 *                PRIMITIVAS  BASICAS DE ITERACIÓN
 * *****************************************************************/
 
// Crea un iterador y lo ubica en el primer elemento de la lista.
// Pre: la lista fue creada.
// Post: devuelve un iterador en el primer elemento de la lista.
lista_iter_t *lista_iter_crear(const lista_t *lista);

// Mueve el iterador al siguiente elemento de la lista. En caso de 
// encontrarse al final de la lista devuelve false, caso contrario
// devuelve true.
// Pre: el iterador fue creado.
// Post: el iterador avanzó al siguiente elemento.
bool lista_iter_avanzar(lista_iter_t *iter);

// Obtiene el valor del elemento en donde se posiciona el iterador, 
// devuelve NULL en caso de estar al final de la lista o si la lista
// está vacía.
// Pre: el iterador fue creado.
// Post: se devolvió el valor del elemento donde está el iterador.
void *lista_iter_ver_actual(const lista_iter_t *iter);

// Devuelve verdadero o falso, segun si el iterador se encuentra al
// final de la lista.
// Pre: el iterador fue creado.
bool lista_iter_al_final(const lista_iter_t *iter);

// Destruye el iterador.
// Pre: el iterador fue creado.
// Post: se destruyó el iterador.
void lista_iter_destruir(lista_iter_t *iter);

/* ******************************************************************
 *              PRIMITIVAS DE LISTAS JUNTO CON ITERADOR
 * *****************************************************************/

// Inserta un nuevo elemento a la lista en la posicion del iterador, 
// devuelve falso en caso de error.
// Pre: la lista y el iterador fueron creados.
// Post: se insertó un nuevo elemento en la lista.
bool lista_insertar(lista_t *lista, lista_iter_t *iter, void *dato);

// Borra de la lista el elemento en donde se encuentra el iterador y 
// devuelve su valor. Devuelve NULL en caso de estar al final de la lista.
// Pre: la lista y el iterador fueron creados
// Post: Se elminó un elemento de la lista y se devolvió su valor. 
void *lista_borrar(lista_t *lista, lista_iter_t *iter);

#endif // LISTA_H
