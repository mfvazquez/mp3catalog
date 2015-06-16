/*
 * linea_de_comando.h
 *
 *  Created on: 05/11/2011
 *      Author: Matías Vázquez
 *      Padron: 91523
 *      Professor: Maximiliano Curia
 */

#ifndef LINEA_DE_COMANDO_H_
#define LINEA_DE_COMANDO_H_

#include "heap.h"
#include "hash.h"

//constantes para el filtro
#define SIN_FILTRO 5
#define ARTISTA 1
#define ANYO 4
#define GENERO 3
#define ALBUM 2

//constantes para el criterio
#define PUNTAJE 1
#define MAS_ESCUCHADAS 2
#define MENOS_ESCUCHADAS 3
#define RANDOM 4

/*
 * Funcion que lee todas las canciones del archivo coleccion y las
 * selecciona en base al filtro y argumento pasados por parametro,
 * las canciones que pasan el filtro son almacenadas en un hash que
 * es devuelto por la funcion.
 * ATENCION: se debe liberar el hash devuelto por la funcion.
 * Pre: filtro debe ser alguno de los filtros definidos en
 * linea_de_comando.h.
 * Post: Se devolvio un hash con las canciones que pasaron el filtro.
 */
hash_t* filtrar(char* coleccion, char* argumento, int filtro);

/*
 * Guarda las canciones del hash a un heap en base al criterio seleccionado.
 * ATENCION: el heap devuelto debe ser liberado.
 * Pre: el hash fue creado y criterio es algun criterio valido definido
 * en linea_de_comando.h
 * Post: se devolvio un heap con las canciones guardadas en base al criterio elegido.
 */
heap_t* aplicar_criterio_comando(hash_t* hash, int criterio);

/*
 * Agrega los puntajes leidos del archivo puntajes a las canciones
 * del hash. Si alguna cancion del hash no se encuentra en el archivo
 * tendra puntajes, votos y reproducciones nulos.
 * Pre: el hash fue creado.
 * Post: Se guardaró el puntaje, votos, y reproducciones a las canciones
 * del hash.
 */
void agregar_puntajes(hash_t* hash, char* puntajes);

/*
 * Funcion que imprime en pantalla "cantidad" de canciones guardadas
 * en el heap de mayor a menor(en base al criterio elegido) si
 * destruir_dato es true destruye las canciones del heap.
 * Pre: el heap fue creado.
 * Post: se imprimieron en pantalla la cantidad de canciones elegidas
 * y se destruyeron las canciones en caso de que destruir_dato sea true.
 */
void listar_canciones(heap_t* heap, int cantidad, bool destruir_dato);

/*
 * Funcion principal de linea_de_comando.c que
 * lee las opciones elegidas por el usuario e irá
 * creando la lista de canciones a mostrar.
 */
void linea_de_comando(int argc, char** argv);

#endif /* LINEA_DE_COMANDO_H_ */
