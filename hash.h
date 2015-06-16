#ifndef HASH_H
#define HASH_H

#include <stdbool.h>
#include <stddef.h>
#include "lista.h"


/* ******************************************************************
 *    			   Profesor: Maximiliano Curia                      *
 *  Integrantes del grupo:                    Padron:               *
 * 		Vázquez Matías..........................91523               *
 *		San Pedro Martín........................91461               *
 * ******************************************************************/

/* ******************************************************************
 *                DEFINICION DE LOS TIPOS DE DATOS                  *
 * ******************************************************************/

typedef struct hash hash_t;
typedef struct hash_iter hash_iter_t;
typedef void (*hash_destruir_dato_t)(void *);

/* ********************************************************************
 *                     PRIMITIVAS DE TABLA DE HASH                    *
 * ********************************************************************/

/* Crea una tabla de hash vacía y guarda la funcion destruir_dato.
 * Pre: la funcion destruir_dato ya está definida. 
 * Post: se devolvió una tabla de hash vacía.*/
hash_t *hash_crear(hash_destruir_dato_t destruir_dato);

/* Destruye la tabla hash ingresada y aplica la funcion destruir_dato
 * a cada elemento guardado en el hash, si destruir_dato es NULL solo
 * destruirá la tabla de hash.
 * Pre: la tabla de hash fué creada.
 * Post: se destruye el hash y se aplico la funcion destruir_dato
 * a cada elemento del hash.*/
void hash_destruir(hash_t *hash);

/* Agrega un nuevo elemento a la tabla de hash. Devuelve falso en caso 
 * de error.
 * Pre: la tabla de hash fué creada.
 * Post: se guardó un nuevo elemento en el hash y devolvió verdadero
 * de tener exito, sino falso.*/
bool hash_guardar(hash_t *hash, const char *clave, void *dato);

/* Devuelve verdadero si el elemento asociado a la clave se encuentra 
 * en la tabla de hash, caso contrario devuelve falso.
 * Pre: la tabla de hash fué creada.
 * Post: se devolvió verdadero o falso.*/
bool hash_pertenece(hash_t *hash, const char *clave);

/* Quita el elemento, correspondiente a la clave, almacenado en la 
 * tabla de hash, o devuelve NULL en caso de no encontrarse almacenado.
 * Pre: El hash fué creado.
 * Post: Se devolvió el dato correspondiente a la clave, o NULL en caso
 * de no encontrar el dato.*/
void *hash_borrar(hash_t *hash, const char *clave);

/* Devuelve el dato guardado en la tabla de hash asociado a la clave o 
 * devuelve NULL en caso de no encontrarse la clave.
 * Pre: el hash fué creado.
 * Post: se devolvió el elemento del hash correspondiente a la clave, o 
 * NULL en caso de no encontrarlo.*/
void *hash_obtener(hash_t *hash, const char *clave); 

/* Devuelve la cantidad de espacios ocupados en la tabla de hash.
 * Pre: la tabla de hash fué creada.
 * Post: se devolvió la cantidad de espacios ocupados en la tabla de hash.*/
size_t hash_cantidad(hash_t *hash);


/* ********************************************************************
 * 						PRIMITIVAS DE ITERACION                       *
 * ********************************************************************/

/* Crea un iterador en la tabla de hash ingresada y lo devuelve.
 * Pre: La tabla de hash fué creada.
 * Post: se devolvió un iterador.*/
hash_iter_t *hash_iter_crear(const hash_t *hash);

/* Avanza el iterador a la siguiente posición de la tabla de hash.
 * Devuelve verdadero si el iter avanzó, o falso si está al final del
 * hash.
 * Pre: el iter fué creado.
 * Post: se devolvió verdadero o falso y el iter avanzço una posicion.*/
bool hash_iter_avanzar(hash_iter_t *iter);

/* Devuelve la clave actual en donde esta posicionado el iterador
 * Pre: El iter fué creado.
 * Post: Se devolvió la clave del elemento en el que se encuentra el iter.*/
const char *hash_iter_ver_actual(const hash_iter_t *iter);

/* Devuelve verdadero si el iter se encuentra al final de la tabla de 
 * hash, devuelve falso en caso contrario
 * Pre: El iterador fué creado.
 * Psot: Se devolvio verdadero o falso.*/
bool hash_iter_al_final(const hash_iter_t *iter);

/* Libera el espacio de la memoria ocupado por el iter
 * Pre: El iter fué creado.
 * Post: Se destruyó el iter.*/
void hash_iter_destruir(hash_iter_t* iter);

#endif // HASH_H

