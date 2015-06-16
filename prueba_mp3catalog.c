/*
 * prueba_mp3catalog.c
 *
 *  Created on: 13/11/2011
 *      Author: matias
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funciones_auxiliares.h"
#include "interactivo.h"
#include "linea_de_comando.h"
#include "lector_canciones.h"

/* Función auxiliar para imprimir si estuvo OK o no. */
void print_test(char* name, bool result)
{
    printf("%s: %s\n", name, result? "OK" : "ERROR");
}

void prueba_funciones_auxiliares(void)
{
	printf("Prueba de funciones auxiliares\n");

	char* clave1 = "minusculassinespacios";
	char* clave2 = "minusculas_con_guion_bajo";
	char* clave3 = "MAYUSCULASSINESPACIOS";
	char* clave4 = "MAYUSCULAS_CON_GUION_BAJO";
	char* clave5 = "MaYUsCulAs_y MiNusCulAs con_y sin GuiOnes_Bajos";

	char* clave1a = minusculas(clave1);
	char* clave1b = reemplazar_(clave1);

	char* clave2a = minusculas(clave2);
	char* clave2b = reemplazar_(clave2);

	char* clave3a = minusculas(clave3);
	char* clave3b = reemplazar_(clave3);

	char* clave4a = minusculas(clave4);
	char* clave4b = reemplazar_(clave4);

	char* clave5a = minusculas(clave5);
	char* clave5b = reemplazar_(clave5);
	char* clave5c = reemplazar_(clave5a);
	printf("clave 5c = %s\n", clave5c);

	print_test("Prueba minusculas clave1", strcmp(clave1a, clave1)==0);
	print_test("Prueba reemplazar_ clave1", strcmp(clave1b, clave1)==0);

	print_test("Prueba minusculas clave2", strcmp(clave2a, clave2)==0);
	print_test("Prueba reemplazar_ clave2", strcmp(clave2b, "minusculas con guion bajo")==0);

	print_test("Prueba minusculas clave3", strcmp(clave3a, "mayusculassinespacios")==0);
	print_test("Prueba reemplazar_ clave3", strcmp(clave3b, clave3)==0);

	print_test("Prueba minusculas clave4", strcmp(clave4a, "mayusculas_con_guion_bajo")==0);
	print_test("Prueba reemplazar_ clave4", strcmp(clave4b, "MAYUSCULAS CON GUION BAJO")==0);

	print_test("Prueba minusculas clave5", strcmp(clave5a, "mayusculas_y minusculas con_y sin guiones_bajos")==0);
	print_test("Prueba reemplazar_ clave5", strcmp(clave5b, "MaYUsCulAs y MiNusCulAs con y sin GuiOnes Bajos")==0);
	print_test("Prueba minusculas y reemplazar_ clave5", strcmp(clave5c, "mayusculas y minusculas con y sin guiones bajos")==0);

	free(clave1a);
	free(clave1b);
	free(clave2a);
	free(clave2b);
	free(clave3a);
	free(clave3b);
	free(clave4a);
	free(clave4b);
	free(clave5a);
	free(clave5b);
	free(clave5c);
}

void prueba_linea_de_comando(void)
{
	char* artista = malloc(5 * sizeof(char));
	char* album = malloc(6 * sizeof(char));
	char* anyo = malloc(5 * sizeof(char));
	char* genero = malloc(3 * sizeof(char));

	strcpy(artista, "slim");
	strcpy(album, "sense");
	strcpy(anyo, "2009");
	strcpy(genero, "13");

	printf("Prueba linea de comando:\n");
	hash_t* hash1 = filtrar("coleccion.txt", NULL, SIN_FILTRO);
	hash_t* hash2 = filtrar("coleccion.txt", artista, ARTISTA);
	hash_t* hash3 = filtrar("coleccion.txt", album, ALBUM);
	hash_t* hash4 = filtrar("coleccion.txt", anyo, ANYO);
	hash_t* hash5 = filtrar("coleccion.txt", genero, GENERO);

	free(artista);
	free(album);
	free(anyo);
	free(genero);

	print_test("Prubeba filtrar sin filtro", hash_cantidad(hash1)==176); // hay un total de 176 canciones
	print_test("Prubeba filtrar por artista=slim", hash_cantidad(hash2)==13); //hay 13 canciones de slim
	print_test("Prubeba filtrar por album=sense", hash_cantidad(hash3)==13); //hay 13 canciones del album sense
	print_test("Prubeba filtrar por año=2009", hash_cantidad(hash4)==62); // hay 62 canciones del 2009
	print_test("Prubeba filtrar por genero=13", hash_cantidad(hash5)==33); // hay 33 canciones de genero 13

	agregar_puntajes(hash1, "puntajes.txt");
	agregar_puntajes(hash2, "puntajes.txt");
	agregar_puntajes(hash3, "puntajes.txt");
	agregar_puntajes(hash4, "puntajes.txt");
	agregar_puntajes(hash5, "puntajes.txt");

	heap_t* heap1 = aplicar_criterio_comando(hash1, RANDOM);
	heap_t* heap2 = aplicar_criterio_comando(hash2, MAS_ESCUCHADAS);
	heap_t* heap3 = aplicar_criterio_comando(hash3, MENOS_ESCUCHADAS);
	heap_t* heap4 = aplicar_criterio_comando(hash4, PUNTAJE);
	heap_t* heap5 = aplicar_criterio_comando(hash5, RANDOM);

	printf("Imprimimos 5 canciones sin filtro y en criterio random:");
	listar_canciones(heap1, 5, true );

	printf("Imprimimos 5 canciones con filtro artista = slim y en criterio mas escuchadas:");
	listar_canciones(heap2, 5, true );

	printf("Imprimimos 5 canciones con filtro album = sense y en criterio menos escuchadas:");
	listar_canciones(heap3, 5, true );

	printf("Imprimimos 5 canciones con filtro año = 2009 y en criterio mayor puntaje:");
	listar_canciones(heap4, 5, true );

	printf("Imprimimos 5 canciones con filtro genero = 13 y en criterio random:");
	listar_canciones(heap5, 5, true );

	hash_destruir(hash1);
	hash_destruir(hash2);
	hash_destruir(hash3);
	hash_destruir(hash4);
	hash_destruir(hash5);

	heap_destruir(heap1, destruir_paquete);
	heap_destruir(heap2, destruir_paquete);
	heap_destruir(heap3, destruir_paquete);
	heap_destruir(heap4, destruir_paquete);
	heap_destruir(heap5, destruir_paquete);
}

void prueba_interactivo(void)
{
	filtros_t* datos = crear_filtros();

	char* artista = malloc(5 * sizeof(char));
	char* album = malloc(6 * sizeof(char));
	char* anyo = malloc(5 * sizeof(char));
	char* genero = malloc(3 * sizeof(char));

	strcpy(artista, "slim");
	strcpy(album, "sense");
	strcpy(anyo, "2009");
	strcpy(genero, "13");

	printf("Prueba interactivo:\n");
	hash_t* hash = filtrar("coleccion.txt", NULL, SIN_FILTRO);

	print_test("Prubeba filtrar sin filtro", hash_cantidad(hash)==176); // hay un total de 176 canciones

	agregar_puntajes(hash, "puntajes.txt");
	guardar_canciones(datos, hash);
	hash_destruir(hash);

	lista_t* lista_sin_filtro = obtener_canciones(datos, SIN_FILTRO, NULL);
	lista_t* lista_artista = obtener_canciones(datos, ARTISTA, artista);
	lista_t* lista_album = obtener_canciones(datos, ALBUM, album);
	lista_t* lista_anyo = obtener_canciones(datos, ANYO, anyo);
	lista_t* lista_genero = obtener_canciones(datos, GENERO, genero);

	free(artista);
	free(album);
	free(anyo);
	free(genero);

	print_test("Prubeba filtrar sin filtro", lista_largo(lista_sin_filtro)==176); // hay un total de 176 canciones
	print_test("Prubeba filtrar por artista=slim", lista_largo(lista_artista)==13); //hay 13 canciones de slim
	print_test("Prubeba filtrar por album=sense", lista_largo(lista_album)==13); //hay 13 canciones del album sense
	print_test("Prubeba filtrar por año=2009", lista_largo(lista_anyo)==62); // hay 62 canciones del 2009
	print_test("Prubeba filtrar por genero=13", lista_largo(lista_genero)==33); // hay 33 canciones de genero 13

	heap_t* heap_sin_filtro = aplicar_criterio_interactivo(lista_sin_filtro, RANDOM);
	heap_t* heap_artista = aplicar_criterio_interactivo(lista_artista, MAS_ESCUCHADAS);
	heap_t* heap_album = aplicar_criterio_interactivo(lista_album, MENOS_ESCUCHADAS);
	heap_t* heap_anyo = aplicar_criterio_interactivo(lista_anyo, PUNTAJE);
	heap_t* heap_genero = aplicar_criterio_interactivo(lista_genero, RANDOM);

	printf("Imprimimos 5 canciones sin filtro y en criterio random:");
	listar_canciones(heap_sin_filtro, 5, false);
	printf("Imprimimos 5 canciones con filtro artista = slim y en criterio mas escuchadas:");
	listar_canciones(heap_artista, 5, false);
	printf("Imprimimos 5 canciones con filtro album = sense y en criterio menos escuchadas:");
	listar_canciones(heap_album, 5, false);
	printf("Imprimimos 5 canciones con filtro año = 2009 y en criterio mayor puntaje:");
	listar_canciones(heap_anyo, 5, false);
	printf("Imprimimos 5 canciones con filtro genero = 13 y en criterio random:");
	listar_canciones(heap_genero, 5, false);

	heap_destruir(heap_sin_filtro, free);
	heap_destruir(heap_artista, free);
	heap_destruir(heap_album, free);
	heap_destruir(heap_anyo, free);
	heap_destruir(heap_genero, free);

	lista_destruir(lista_sin_filtro, NULL); // solo hay que destruir una lista si el filtro es SIN_FILTRO

	destruir_filtros(datos);
}

int main(void)
{
	prueba_funciones_auxiliares();
	prueba_linea_de_comando();
	prueba_interactivo();
	return 0;
}
