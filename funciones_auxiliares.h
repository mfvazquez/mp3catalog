/*
 * funciones_auxiliares.h
 *
 *  Created on: 05/11/2011
 *      Author: Matías Vázquez
 *      Padron: 91523
 *      Professor: Maximiliano Curia
 */

#ifndef FUNCIONES_AUXILIARES_H_
#define FUNCIONES_AUXILIARES_H_

#include <stdlib.h>
#include <stddef.h>
#include "lector_canciones.h"
#include "heap.h"

typedef struct paquete{
	int clave;
	cancion_t* cancion;
}paquete_t;

/*
 * Funcion auxiliar para ingreso de texto por teclado
 * pre: Se debe ingresar el tamaño inicial del arreglo.
 * post: Devuelve un puntero a una porcion de memoria con el texto indicado.
 * En caso de fallar, devuelve NULL.
 * Si lee EOF y no se ingresó texto devuelve NULL.
 * Nota: La memoria es reservada dentro de la funcion, el usuario deberia
 * encargarse de liberarla.
 */
char* leer_texto(size_t tam);

/*
 * Funcion que reemplaza los '_' por espacios.
 *
 * ATENCION: La memoria pedida para el nuevo arreglo debe ser liberada.
 */
char* reemplazar_(char* arreglo);

/*
 * Funcion que devuelve un arreglo en minusculas del arreglo pasado.
 *
 * ATENCION: La memoria pedida para el nuevo arreglo debe ser liberada.
 */
char* minusculas(char* arreglo);

/*
 * Guarda el puntero de la cancion ingresada en una estructura paquete y
 * le asigna un numero (clave) del tipo int como referencia.
 */
paquete_t* empaquetar(cancion_t* cancion, int clave);

/*
 * Destruye el paquete y devuelve la cancion.
 */
void* desempaquetar(paquete_t* paquete);

/*
 * Funcion de comparacion de las claves de los paquetes,
 * devuelve 1 si la clave del paquete a es mayor, -1 si es
 * menor y 0 si son iguales.
 * pre: los paquetes fueron creados.
 * post: se devolvio un int.
 */
int comparar(paquete_t* a, paquete_t* b);

/*
 * Libera el espacio de memoria ocupado por el paquete y la cancion.
 * Pre: el paquete fue creado.
 * Post: se libero el espacio de la memoria ocupado por el paquete y
 * la cancion.
 */
void destruir_paquete(void* paquete);

/*
 * Copia el genero de la cancion que es de tipo unsigned char
 * a una cadena de caracteres y la deveuvle.
 * Atencion: la cadena debe ser liberada por el usuario.
 * Pre: la cancion fue creada.
 * Post: se devolvio una cadena de caracteres.
 */
char* copiar_genero(cancion_t* cancion);

#endif /* FUNCIONES_AUXILIARES_H_ */
