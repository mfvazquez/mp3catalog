/*
 * heap.c
 *
 *  Created on: 23/10/2011
 *      Author: matias
 *      Padron: 91523
 *      Professor: Maximiliano Curia
 */

#include <stdlib.h>
#include "heap.h"

#define LONG_INI 256

/*
 * Implementación de un TAD cola de prioridad, usando un max-heap.
 *
 * Notar que al ser un max-heap el elemento mas grande seró el de mejor
 * prioridad. Si se desea un min-heap, alcanza con invertir la función de
 * comparación.
 */

/* ******************************************************************
 *                DEFINICION DE LOS TIPOS DE DATOS
 * ******************************************************************/

/* Tipo utilizado para el heap. */
struct heap_t{
	cmp_func_t cmp;
	void** vector;
	size_t cantidad;
	size_t tam;
};

/* ******************************************************************
 * 						FUNCIONES AUXILIARES
 * ******************************************************************/

/* La funcion swap intercambia dos punteros de un vector. */
void swap (void* vector[], int x, int y)
{
	void* temp = vector[x];

	vector[x] = vector[y];
	vector[y] = temp;
}

/*
 * Funcion recursiva que luego de haberse guardado un dato en el vector,
 * lo reordena de forma que sigua cumpliendo la condición de heap
 * Pre: cmp debe estar definida
 * Post: Se reordenó el vector.
 */
void upheap(void* vector[], size_t pos, cmp_func_t cmp)
{
	if (pos == 0) return;

	size_t pos_padre = ((pos - 1) / 2);
	if (cmp(vector[pos_padre], vector[pos]) < 0){
		swap(vector, pos_padre, pos);
		upheap(vector, pos_padre, cmp);
	}
}

/*
 * Funcion recursiva que luego de haberse borrado el primer elemento del vector,
 * lo reordena de forma que sigua cumpliendo la condición de heap.
 * Pre: cmp debe estar definida.
 * Post: Se reordenó el vector.
 */
void downheap(void* vector[], size_t pos, size_t cantidad, cmp_func_t cmp)
{;
	size_t hijo_izq = (pos * 2) + 1;
	if (hijo_izq >= cantidad) return;  // no existe el hijo izquierdo

	if (hijo_izq == cantidad - 1){ // hijo izquierdo esta al final del vector
		if (cmp(vector[pos], vector[hijo_izq]) < 0)
				swap(vector, pos, hijo_izq);
		return;
	}
	size_t hijo_der = hijo_izq + 1;
	size_t hijo_mayor = (cmp(vector[hijo_izq], vector[hijo_der]) > 0)? hijo_izq : hijo_der;

	if (cmp(vector[pos], vector[hijo_mayor]) < 0){
		swap(vector, pos ,hijo_mayor);
		downheap(vector, hijo_mayor, cantidad, cmp);
	}
}

/*
 * Funcion que recibe un vector y lo ordena de forma que cumpla la condicion
 * de heap.
 * Pre: cmp debe estar definida.
 * Post: se reordenó el vector.
 */
void heapify(void* vector[], size_t len, cmp_func_t cmp)
{
	for(int i = (len / 2) - 1; i >= 0; i--)
		downheap(vector, i, len, cmp);
}

/*
 * Redimensiona el vector del heap al nuevo tamaño ingresado y devuelve true.
 * En caso de error devuelve false.
 * Pre: El heap debe estar creado, nuevo_tam debe ser mayor o igual que 0.
 * Post: Se redimensionó el vector del heap y se devolvió true, o false en
 * caso de error
 */
bool heap_redimensionar(heap_t* heap, size_t nuevo_tam)
{
	void** vector_aux = realloc(heap->vector, nuevo_tam * sizeof(void*));
	if (!vector_aux) return false;
	heap->vector = vector_aux;
	heap->tam = nuevo_tam;
	return true;
}

/* ******************************************************************
 * 							HEAPSORT
 * ******************************************************************/

/* Función de heapsort genérica. Esta función ordena mediante heapsort
 * un arreglo de punteros opacos, para lo cual requiere que se
 * le pase una función de comparación. Modifica el arreglo "in-place".
 * Notar que esta función NO es formalmente parte del TAD Heap.
 */
void heapsort(void *elementos[], size_t len, cmp_func_t cmp)
{
	heapify(elementos, len, cmp);
	for (int pos = len -1;; pos--){		// recorremos el vector desde el final hasta la posicion 2
		swap(elementos, pos, 0);	// intercambiamos los valores de la posicion 0 con la de pos
		if (len  == 2) break;
		downheap(elementos, 0, --len, cmp);		// aplicamos downheap al valor de la posicion 0, reduciendo el rango de accion del vector una posicion menos en cada pasada
	}
}

/* ******************************************************************
 * 					PRIMITIVAS  BASICAS DEL HEAP
 * ******************************************************************/

/* Crea un heap. Recibe como único parámetro la función de comparación a
 * utilizar. Devuelve un puntero al heap, el cual debe ser destruido con
 * heap_destruir().
 */
heap_t *heap_crear(cmp_func_t cmp)
{
	heap_t* heap = malloc(sizeof(heap_t));
	if (!heap) return NULL;

	heap->vector = malloc(LONG_INI * sizeof(void*));
	if (!heap->vector){
		free(heap);
		return NULL;
	}

	heap->cmp = cmp;
	heap->cantidad = 0;
	heap->tam = LONG_INI;

	return heap;
}

/* Elimina el heap, llamando a la función dada para cada elemento del mismo.
 * El puntero a la función puede ser NULL, en cuyo caso no se llamará.
 * Post: se llamó a la función indicada con cada elemento del heap. El heap
 * dejó de ser válido. */
void heap_destruir(heap_t *heap, void destruir_elemento(void *e))
{
	if (destruir_elemento){
		for (int i = 0; i < heap->cantidad; i++){
			if (heap->vector[i])
				destruir_elemento(heap->vector[i]);
		}
	}
	free(heap->vector);
	free(heap);
}

/* Devuelve la cantidad de elementos que hay en el heap. */
size_t heap_cantidad(const heap_t *heap)
{
	return heap->cantidad;
}

/* Devuelve true si la cantidad de elementos que hay en el heap es 0, false en
 * caso contrario. */
bool heap_esta_vacio(const heap_t *heap)
{
	return heap->cantidad == 0;
}

/* Agrega un elemento al heap. El elemento no puede ser NULL.
 * Devuelve true si fue una operación exitosa, o false en caso de error.
 * Pre: el heap fue creado.
 * Post: se agregó un nuevo elemento al heap.
 */
bool heap_encolar(heap_t *heap, void *elem)
{
	heap->vector[heap->cantidad] = elem;
	upheap(heap->vector,heap->cantidad, heap->cmp);
	heap->cantidad++;

	if (heap->cantidad == heap->tam)
		if(!heap_redimensionar(heap, heap->tam * 2))
			return false;
	return true;
}

/* Devuelve el elemento con máxima prioridad. Si el heap esta vacío, devuelve
 * NULL.
 * Pre: el heap fue creado.
 */
void *heap_ver_max(const heap_t *heap)
{
	return (heap->cantidad == 0)? NULL : heap->vector[0];
}

/* Elimina el elemento con máxima prioridad, y lo devuelve.
 * Si el heap esta vacío, devuelve NULL.
 * Pre: el heap fue creado.
 * Post: el elemento desencolado ya no se encuentra en el heap.
 */
void *heap_desencolar(heap_t *heap)
{
	if (heap->cantidad == 0) return NULL;

	void* elem = heap->vector[0];
	heap->vector[0] = heap->vector[heap->cantidad - 1];
	heap->cantidad--;

	downheap(heap->vector, 0, heap->cantidad, heap->cmp);

	if ((heap->tam / 4) >= heap->cantidad && heap->tam > LONG_INI)
		heap_redimensionar(heap, heap->tam / 2);

	return elem;
}
