/*
 * funciones_auxiliares.c
 *
 *  Created on: 05/11/2011
 *      Author: Matías Vázquez
 *      Padron: 91523
 *      Professor: Maximiliano Curia
 */

#include "funciones_auxiliares.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>

/*
 * Funcion auxiliar para ingreso de texto por teclado
 * pre: Se debe ingresar el tamaño inicial del arreglo.
 * post: Devuelve un puntero a una porcion de memoria con el texto indicado.
 * En caso de fallar, devuelve NULL.
 * Si lee EOF y no se ingresó texto devuelve NULL.
 * Nota: La memoria es reservada dentro de la funcion, el usuario deberia
 * encargarse de liberarla.
 */
char* leer_texto(size_t tam)
{
	int c, i = 0;
	char* texto = malloc(tam * sizeof(char));
	texto[0] = -1;

	for(i = 0; (c = getchar()) != '\n' && c != EOF; i++){

		if(i + 1 == tam){
			tam += tam;
			char* aux = realloc(texto, tam * sizeof(char));
			if(!aux) break;
			texto = aux;
		}

		texto[i] = c;
	}
	if(c == EOF && texto[0] == -1){  //si texto[0] == -1 no se ingresó texto
		free(texto);
		return NULL;
	}
	texto[i]='\0';
	return texto;
}

/*
 * Funcion que reemplaza los '_' por espacios.
 * 
 * ATENCION: La memoria pedida para el nuevo arreglo debe ser liberada.
 */ 
char* reemplazar_(char* arreglo)
{
	size_t len = strlen(arreglo);
	char* string = malloc(len + 1 * sizeof(char));
	for (int n = 0; n < len; n++)
		string[n] = (arreglo[n] == '_')? ' ':arreglo[n];
	string[len] = '\0';
	return string;
}



/*
 * Funcion que devuelve un arreglo en minusculas del arreglo pasado.
 * 
 * ATENCION: La memoria pedida para el nuevo arreglo debe ser liberada. 
 */
char* minusculas(char* arreglo)
{
	size_t len = strlen(arreglo);
	char* string = malloc(len + 1 * sizeof(char));
	for (int n = 0; n < len; n++)
		string[n] = tolower(arreglo[n]);
	string[len] = '\0';
	return string;
}

/*
 * Guarda el puntero de la cancion ingresada en una estructura paquete y
 * le asigna un numero (clave) del tipo int como referencia.
 */
paquete_t* empaquetar(cancion_t* cancion, int clave)
{
	paquete_t* paquete = malloc(sizeof(paquete_t));
	if(!paquete) return NULL;
	paquete->cancion = cancion;
	paquete->clave = clave;

	return paquete;
}

/*
 * Destruye el paquete y devuelve la cancion.
 */
void* desempaquetar(paquete_t* paquete)
{
	cancion_t* cancion = paquete->cancion;
	free(paquete);
	return cancion;
}

/*
 * Funcion de comparacion de las claves de los paquetes,
 * devuelve 1 si la clave del paquete a es mayor, -1 si es
 * menor y 0 si son iguales.
 * pre: los paquetes fueron creados.
 * post: se devolvio un int.
 */
int comparar(paquete_t* a, paquete_t* b)
{
	if(a->clave > b->clave) return 1;
	if(a->clave < b->clave) return -1;
	return 0;
}

/*
 * Libera el espacio de memoria ocupado por el paquete y la cancion.
 * Pre: el paquete fue creado.
 * Post: se libero el espacio de la memoria ocupado por el paquete y
 * la cancion.
 */
void destruir_paquete(void* paquete)
{
	cancion_t* cancion = desempaquetar(paquete);
	cancion_destruir(cancion);
}

/*
 * Copia el genero de la cancion que es de tipo unsigned char
 * a una cadena de caracteres y la deveuvle.
 * Atencion: la cadena debe ser liberada por el usuario.
 * Pre: la cancion fue creada.
 * Post: se devolvio una cadena de caracteres.
 */
char* copiar_genero(cancion_t* cancion)
{
	char* genero = malloc(4 * sizeof(char));
	sprintf(genero, "%3u", cancion->genero);
	return genero;
}
