/*
 * linea_de_comando.c
 *
 *  Created on: 05/11/2011
 *      Author: Matías Vázquez
 *      Padron: 91523
 *      Professor: Maximiliano Curia
 */

#include "funciones_auxiliares.h"
#include "linea_de_comando.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>

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
hash_t* filtrar(char* coleccion, char* argumento, int filtro)
{
	hash_t* hash = hash_crear(NULL);
	char* clave = NULL;
	if(argumento) clave = reemplazar_(argumento); // reemplazamos los _ por espacios
	lector_canciones_t* lector = lector_canciones_crear(coleccion, LECTOR_COLECCION);
	if (!lector){
		printf("Error al leer el archivo %s.\n", coleccion);
		hash_destruir(hash);
		return NULL;
	}
	cancion_t* cancion;
	while(!lector_canciones_al_final(lector)){
		cancion = lector_canciones_ver_actual(lector);
		
		// Guardamos en la lista el dato segun el filtro elegido
		if (filtro == ARTISTA){
			char* artista = minusculas(cancion->artista);
			if(strcmp(artista, clave)==0)
				hash_guardar(hash, cancion->archivo, cancion);
			else 
				cancion_destruir(cancion);
			free(artista);
			
		}else if (filtro == ALBUM){
			char* album = minusculas(cancion->album);
			if (strcmp(album, clave)==0)
				hash_guardar(hash, cancion->archivo, cancion);
			else 
				cancion_destruir(cancion);
			free(album);
			
		}else if (filtro == ANYO && strcmp(cancion->anyo, clave)==0){
			hash_guardar(hash, cancion->archivo, cancion);
		}
		else if (filtro == GENERO && cancion->genero == atoi(clave))
			hash_guardar(hash, cancion->archivo, cancion);
		
		else if (filtro == SIN_FILTRO)
			hash_guardar(hash, cancion->archivo, cancion);
		
		else
			cancion_destruir(cancion);
		lector_canciones_avanzar(lector);
	}
	lector_canciones_destruir(lector);
	if(clave) free(clave);
	return hash;
}

/*
 * Guarda las canciones del hash a un heap en base al criterio seleccionado.
 * ATENCION: el heap devuelto debe ser liberado.
 * Pre: el hash fue creado y criterio es algun criterio valido definido
 * en linea_de_comando.h
 * Post: se devolvio un heap con las canciones guardadas en base al criterio elegido.
 */
heap_t* aplicar_criterio_comando(hash_t* hash, int criterio)
{
	heap_t* heap = heap_crear((cmp_func_t)comparar);
	paquete_t* paquete;
	cancion_t* cancion;
	int valor;
	hash_iter_t* iter = hash_iter_crear(hash);
	while(!hash_iter_al_final(iter)){
		const char* clave = hash_iter_ver_actual(iter);		
		cancion = hash_obtener(hash, clave);

		if(criterio == PUNTAJE) valor = cancion->puntos;
		else if(criterio == MAS_ESCUCHADAS) valor = cancion->reproducciones;
		else if(criterio == MENOS_ESCUCHADAS) valor = -1 *(cancion->reproducciones);
		else if(criterio == RANDOM) valor = rand();

		paquete = empaquetar(cancion, valor);
		heap_encolar(heap, paquete);
		hash_iter_avanzar(iter);
	}
	hash_iter_destruir(iter);
	return heap;
}

/*
 * Agrega los puntajes leidos del archivo puntajes a las canciones
 * del hash. Si alguna cancion del hash no se encuentra en el archivo
 * tendra puntajes, votos y reproducciones nulos.
 * Pre: el hash fue creado.
 * Post: Se guardaró el puntaje, votos, y reproducciones a las canciones
 * del hash.
 */
void agregar_puntajes(hash_t* hash, char* puntajes)
{
	cancion_t* cancion;
	lector_canciones_t* lector = lector_canciones_crear(puntajes, LECTOR_PUNTAJES);
	if (!lector){
		printf("Error al leer el archivo %s.\n", puntajes);
		return;
	}
	puntaje_cancion_t* puntaje;
	
	while(!lector_canciones_al_final(lector)){
		puntaje = lector_canciones_ver_actual(lector);
		cancion = hash_obtener(hash, puntaje->archivo);
		if(cancion){
			cancion->puntos = puntaje->puntos;
			cancion->votos = puntaje->votos;
			cancion->reproducciones = puntaje->reproducciones;
		}
		puntaje_destruir(puntaje);
		lector_canciones_avanzar(lector);
	}
	lector_canciones_destruir(lector);
}

/*
 * Funcion que imprime en pantalla "cantidad" de canciones guardadas
 * en el heap de mayor a menor(en base al criterio elegido) si
 * destruir_dato es true destruye las canciones del heap.
 * Pre: el heap fue creado.
 * Post: se imprimieron en pantalla la cantidad de canciones elegidas
 * y se destruyeron las canciones en caso de que destruir_dato sea true.
 */
void listar_canciones(heap_t* heap, int cantidad, bool destruir_dato)
{
	cancion_t* cancion;
	paquete_t* paquete;
	int contador = 0;
	while(cantidad != 0 && !heap_esta_vacio(heap)){
        paquete = heap_desencolar(heap);
        cancion = desempaquetar(paquete);
        
		printf("\n");
		printf("Archivo: %s\n", cancion->archivo);
        printf("Titulo: %s\n", cancion->titulo);
        printf("Artista: %s\n", cancion->artista);
        printf("Album: %s\n", cancion->album);
        printf("Año: %s\n", cancion->anyo);
        printf("Genero: %d\n", cancion->genero);
        printf("Comentario: %s\n", cancion->comentario);
		printf("Puntos: %d\n", cancion->puntos);
		printf("Votos: %d\n", cancion->votos);
		printf("Reproducciones: %d\n", cancion->reproducciones);
        cantidad--;
        if (destruir_dato) cancion_destruir(cancion);
        contador++;
	}
	printf("\nCanciones encontradas: %d\n\n", contador);
}

/*
 * Funcion principal de linea_de_comando.c que
 * lee las opciones elegidas por el usuario e irá
 * creando la lista de canciones a mostrar.
 */
void linea_de_comando(int argc, char** argv)
{
	// variables a utilizar
	const char* op_cortas = "c:p:C:a:A:g:l:PmMr";
	const struct option op_largas[] ={
		{"coleccion",			1,	NULL,	 'c'},
		{"puntajes",			1,	NULL,	 'p'},
		{"cantidad",			1,	NULL,	 'C'},
		{"artista",				1,	NULL,	 'a'},
		{"anyo",				1,	NULL,	 'A'},
		{"genero",				1,	NULL,	 'g'},
		{"album",				1,	NULL,	 'l'},
		{"por-puntaje",			0,	NULL,	 'P'},
		{"mas-escuchadas",		0,	NULL,	 'm'},
		{"menos-escuchadas",	0,	NULL,	 'M'},
		{"random",				0,	NULL,	 'r'},
		{0,                     0,  0,        0 },
	};
	char* coleccion = "coleccion.txt";
	char* puntajes = "puntajes.txt";
	int filtro = SIN_FILTRO;
	char* argumento_filtro = NULL;
	int criterio = RANDOM; // por omisión listara en orden aleatorio
	char* cantidad_char = "-1"; // por omisión listara todas las canciones
	int cantidad = -1;
	int opcion;

	while(1){
		opcion = getopt_long(argc, argv, op_cortas, op_largas, NULL);
		if (opcion == -1) break;
		switch(opcion){
			case 'c':
				coleccion = optarg;
				break;
			case 'p':
				puntajes = optarg;
				break;
			case 'C':
				cantidad_char = optarg;
				cantidad = atoi(cantidad_char);
				break;
			case 'a':
				if (filtro == ALBUM || filtro == ARTISTA) free(argumento_filtro);
				filtro = ARTISTA;
				argumento_filtro = minusculas(optarg);
				break;
			case 'A':
				if (filtro == ALBUM || filtro == ARTISTA) free(argumento_filtro);
				filtro = ANYO;
				argumento_filtro = optarg;
				break;
			case 'l':
				if (filtro == ALBUM || filtro == ARTISTA) free(argumento_filtro);
				filtro = ALBUM;
				argumento_filtro = minusculas(optarg);
				break;
			case 'g':
				if (filtro == ALBUM || filtro == ARTISTA) free(argumento_filtro);
				filtro = GENERO;
				argumento_filtro = optarg;
				break;
			case 'P':
				criterio = PUNTAJE;
				break;
			case 'm':
				criterio = MAS_ESCUCHADAS;
				break;
			case 'M':
				criterio = MENOS_ESCUCHADAS;
				break;
			case 'r':
				criterio = RANDOM;
				break;
			default:
				printf("Opcion inválida.\n");
				return;
		}
	}
	hash_t* hash = filtrar(coleccion, argumento_filtro, filtro);
	if (!hash) return;
	agregar_puntajes(hash, puntajes);
	if (filtro == ALBUM || filtro == ARTISTA) free(argumento_filtro);
	heap_t* heap = aplicar_criterio_comando(hash, criterio);
	hash_destruir(hash);
	listar_canciones(heap, cantidad, true);
	heap_destruir(heap, destruir_paquete);
}
