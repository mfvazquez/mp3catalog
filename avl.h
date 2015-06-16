/*
 * avl.h
 *
 *  Created on: 8/11/2011
 *      Author: Matías Vázquez
 *      Padron: 91523
 *      Professor: Maximiliano Curia
 */

#ifndef AVL_H_
#define AVL_H_

#include <stdbool.h>
#include <stddef.h>
#include "pila.h"

/* *****************************************************************
 *                DEFINICION DE LOS TIPOS DE DATOS
 * *****************************************************************/

typedef struct avl avl_t;

/*
 * Tipo de función que compara dos arreglos de caracteres,
 * devuelve 1 si el primer arreglo es mayor, 0 si son iguales
 * o -1 en caso de que el segundo sea mayor.
 */
typedef int (*avl_comparar_clave_t) (const char *, const char *);

/*
 * Tipo de función que destruye los datos guardados en el avl.
 */
typedef void (*avl_destruir_dato_t) (void *);

typedef struct avl_iter avl_iter_t;

/* *****************************************************************
 *                 PRIMITIVAS  BASICAS DEL AVL
 * *****************************************************************/

/*
 * Crea un avl y guarda las funciones cmp y destruir_dato.
 * Pre: cmp y destruir_dato deben estar declaradas.
 * Post: se devolvió un avl y se guardaron las funciones.
 */
avl_t* avl_crear(avl_comparar_clave_t cmp, avl_destruir_dato_t destruir_dato);

/*
 * Guarda el dato ingresado en el avl y devuelve true. En caso de error
 * devuelve false.
 * Pre: El avl debe estar creado.
 * Post: Se guardó un dato en el avl y se devolvió true, o false en caso de error.
 */
bool avl_guardar(avl_t *arbol, const char *clave, void *dato);

/*
 * Borra del avl el dato correspondiente a la clave ingresada y lo devuelve.
 * En caso de no encontrarse el dato, devuelve NULL.
 * Pre: El avl fue creado.
 * Post: Se borró del avl el dato correspondiente a la clave ingresada y fue devuelto,
 * en caso de no encontrarse se devolvió NULL.
 */
void *avl_borrar(avl_t *arbol, const char *clave);

/*
 * Devuelve el dato guardado en el avl correspondiente a la clave
 * ingresada. En caso de no encontrarse la clave devuelve, NULL.
 * Pre: El avl fue creado.
 * Post: Se devolvió el dato correspondiente a la clave ingresada, en caso
 * de no encontrarse se devolvió NULL.
 */
void *avl_obtener(const avl_t *arbol, const char *clave);

/*
 * Devuelve true si se encuentra un dato en el avl correspondiente
 * a la clave ingresada, en caso de no encontrarse devuelve false.
 * Pre: El avl fue creado.
 * Post: Se devolvió true si se encontró un dato en el avl correspondiente
 * a la clave, caso contrario se devolvió false
 */
bool avl_pertenece(const avl_t *arbol, const char *clave);

/*
 * Devuelve la cantidad de elementos guardados en el avl.
 * Pre: El avl fue creado.
 * Post: Se devolvió la cantidad de elementos almacenados en el avl.
 */
size_t avl_cantidad(avl_t *arbol);

/*
 * Devuelve la altura de la raiz del arbol.
 * Pre: El avl fue creado.
 * Post: Se devolvió la altura de la raiz del arbol.
 */
size_t avl_altura(avl_t *arbol);

/*
 * Destruye el avl y aplica la función destruir_dato a cada dato almacenado
 * en el avl.
 * Pre: El avl fue creado.
 * Post: Se liberó el espacio ocupado en la memoria por el avl.
 */
void avl_destruir(avl_t *arbol);

/* *****************************************************************
 *                PRIMITIVAS  BASICAS DE ITERACIÓN
 * *****************************************************************/

/*
 * Recorre el avl en inorder, aplicandole a todos los datos la funcion pasada por parametro.
 * Pre: La función debe estar definida.
 * Post: Se aplico la funcion a cada dato del avl.
 */
void avl_in_order(avl_t *arbol, bool funcion(const char *, void *, void *), void *extra);

/*
 * Crea un iterador en el avl. Recorrerá el arbol en inorder.
 * Devuelve NULL en caso de error.
 * Pre: EL arbol fue creado.
 * Post: Se devolvió un iterador, o NULL en caso de error.
 */
avl_iter_t *avl_iter_in_crear(const avl_t *arbol);

/*
 * Avanza el iterador a la siguiente posición y devuelve true.
 * Devuelve false si ya se encuentra al final del arbol.
 * Pre: el iterador fue creado.
 * Post: Se devolvió true y el iter avanzó de posición, o bien
 * se devolvió false.
 */
bool avl_iter_in_avanzar(avl_iter_t *iter);

/*
 * Devuelve la clave del nodo en el que esta posicionado el iterador
 * o NULL si ya se encuentra al final.
 * Pre: El iter fue creado.
 * Post: Se devolvió la clave del nodo actual o NULL si el iter está al final.
 */
const char *avl_iter_in_ver_actual(const avl_iter_t *iter);

/*
 * Devuelve la pendiente del elemento actual del iter.
 * Pre: el iter fue creado.
 * Post: se devolvió la pendiente del elemento actual.
 */
int avl_iter_in_ver_pendiente(const avl_iter_t* iter);

/*
 * Devuelve true si el iter está al final del avl, false en caso contrario.
 * Pre: El iter fue creado.
 * Post: se devolvio true o false, segun corresponda.
 */
bool avl_iter_in_al_final(const avl_iter_t *iter);

/*
 * Destruye el iter, liberando el espacio de la memoria reservado.
 * Pre: El iter fue creado.
 * Post: se destruyó el iter.
 */
void avl_iter_in_destruir(avl_iter_t* iter);

#endif /* AVL_H_ */
