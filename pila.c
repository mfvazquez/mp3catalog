/*
 * pila.c
 *
 *  Created on: ~
 *      Author: Matías Vázquez
 *      Padron: 91523
 *      Professor: Maximiliano Curia
 */


#include <stdlib.h>
#include "pila.h"

/* ******************************************************************
 *                DEFINICION DE LOS TIPOS DE DATOS
 * *****************************************************************/

struct _pila {
    void** datos;
    size_t tamanio;
    size_t cantidad;
};

/* ******************************************************************
 *                        IMPLEMENTACION
 * *****************************************************************/

#define ESPACIO 20  //tamanio inicial de las pilas

// Crea una pila.
// Post: devuelve una nueva pila vacía.

pila_t* pila_crear()
{
	pila_t *pila = malloc(sizeof(pila_t));
	
	if (pila == NULL)
		return NULL;
		
	pila->datos = malloc(ESPACIO * sizeof(void*));
	if (pila->datos == NULL){
		free(pila);
		return NULL;
		
	}
	
	pila->tamanio = ESPACIO;
	pila->cantidad = 0;		

	return pila;
}

// Destruye la pila.
// Pre: la pila fue creada.
// Post: se eliminaron todos los elementos de la pila.

void pila_destruir(pila_t *pila)
{
	free(pila->datos);
	free(pila);
}

// Devuelve verdadero o falso, según si la pila tiene o no elementos apilados.
// Pre: la pila fue creada.

bool pila_esta_vacia(const pila_t *pila)
{
	return pila->cantidad == 0;
}


// Modifica el tamaño de la pila
// Pre: la pila fue creada
// Post: se modificó el tamaño de la pila, en caso de error devuelve false
// sino true.
bool pila_redimensionar(pila_t *pila, size_t nuevo_tamanio)
{
	void* aux = realloc(pila->datos, nuevo_tamanio * sizeof(void *));
	if ( nuevo_tamanio && (aux == NULL)) return false;
	pila->datos = aux;
	pila->tamanio = nuevo_tamanio;
	return true;
}

// Agrega un nuevo elemento a la pila. Devuelve falso en caso de error.
// Pre: la pila fue creada.
// Post: se agregó un nuevo elemento a la pila, valor es el nuevo tope.
bool pila_apilar(pila_t *pila, void* valor)
{
	if (pila->tamanio == pila->cantidad) {
		// vector lleno, se duplica el espacio pedido
		if ( !pila_redimensionar(pila, pila->tamanio * 2) )
			return false;
	}
	pila->datos[pila->cantidad] = valor;
	pila->cantidad++;
	return true;
}

// Obtiene el valor del tope de la pila. Si la pila tiene elementos,
// se devuelve el valor del tope. Si está vacía devuelve NULL.
// Pre: la pila fue creada.
// Post: se devolvió el valor del tope de la pila, cuando la pila no está
// vacía, NULL en caso contrario.

void* pila_ver_tope(const pila_t *pila)
{
	size_t cantidad;
	
	cantidad = pila->cantidad;
	if (cantidad == 0)
		return NULL;

	else
		return pila->datos[cantidad-1];
}

// Saca el elemento tope de la pila. Si la pila tiene elementos, se quita el
// tope de la pila, y se devuelve ese valor. Si la pila está vacía, devuelve 
// NULL.
// Pre: la pila fue creada.
// Post: si la pila no estaba vacía, se devuelve el valor del tope anterior 
// y la pila contiene un elemento menos.

void* pila_desapilar(pila_t *pila)
{
	void* tope = pila_ver_tope(pila);
	if (!tope)
		return NULL;

	//si el espacio sobrante es mayor a cuatro tercios de cantidad y 
	//cantidad es mayor que 20 restamos a tamanio la mitad de cantidad
	if ( pila->cantidad <= (pila->tamanio/4) && pila->cantidad > 20) {
		if ( !pila_redimensionar(pila, pila->tamanio / 2) )
			return false;
	}

	pila->cantidad--;
	return tope;
}
