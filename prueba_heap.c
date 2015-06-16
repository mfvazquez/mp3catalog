/*
 * prueba_heap.c
 *
 *  Created on: 25/10/2011
 *      Author: matias
 *      Padron: 91523
 *      Professor: Maximiliano Curia
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "heap.h"

/* ******************************************************************
 *                      FUNCIONES AUXILIARES
 * *****************************************************************/

/* Función auxiliar para imprimir si estuvo OK o no. */
void print_test(char* name, bool result)
{
    printf("%s: %s\n", name, result? "OK" : "ERROR");
}

int cmp(int* x, int* y)
{
	if (*x > *y) return 1;
	if (*x < *y) return -1;
	return 0;
}

/* ******************************************************************
 *                        PRUEBAS UNITARIAS
 * *****************************************************************/

/* Prueba que las primitivas funcionen correctamente. */
void prueba_heap_crear_vacio(void)
{
	heap_t* heap = heap_crear(NULL);

	print_test("Prueba heap crear heap vacio", heap);
	print_test("Prueba heap la cantidad de elementos es 0", heap_cantidad(heap) == 0);
	print_test("Prueba heap desencolar, es NULL, no existe", !heap_desencolar(heap));
	print_test("Prueba heap esta vacio es true", heap_esta_vacio(heap));
	print_test("Prueba heap ver max es NULL, no existe", !heap_ver_max(heap));
	print_test("Prueba heap ver max es NULL, no existe", !heap_ver_max(heap));

	heap_destruir(heap, NULL);
}

void prueba_heap_encolar(void)
{
	heap_t* heap = heap_crear((cmp_func_t) strcmp);

	char* clave0 = "00";
	char* clave1 = "05";
	char* clave2 = "03";
	char* clave3 = "80";
	char* clave4 = "50";
	char* clave5 = "01";
	char* clave6 = "90";

	/* Inserta 1 valor y luego lo borra */
	print_test("Prueba heap encolar clave0", heap_encolar(heap, clave0));
	print_test("Prueba heap la cantidad de elementos es 0", heap_cantidad(heap) == 1);
	print_test("Prueba heap ver max es clave0", heap_ver_max(heap) == clave0);
	print_test("Prueba heap ver max es clave0", heap_ver_max(heap) == clave0);
	print_test("Prueba heap obtener clave0", heap_desencolar(heap));
	print_test("Prueba heap obtener clave0 es NULL, no existe", !heap_desencolar(heap));
	print_test("Prueba heap la cantidad de elementos es 0", heap_cantidad(heap) == 0);

	/* Inserta otros valores y no los borra */
	print_test("Prueba heap encolar clave1", heap_encolar(heap, clave1));
	print_test("Prueba heap la cantidad de elementos es 1", heap_cantidad(heap) == 1);
	print_test("Prueba heap ver max es clave1", heap_ver_max(heap) == clave1);
	print_test("Prueba heap ver max es clave1", heap_ver_max(heap) == clave1);
	print_test("Prueba heap esta vacio es false", !heap_esta_vacio(heap));


	print_test("Prueba heap encolar clave2", heap_encolar(heap, clave2));
	print_test("Prueba heap la cantidad de elementos es 2", heap_cantidad(heap) == 2);
	print_test("Prueba heap ver max es clave1, es mayor a clave2", heap_ver_max(heap) == clave1);
	print_test("Prueba heap ver max es clave1, es mayor a clave2", heap_ver_max(heap) == clave1);
	print_test("Prueba heap esta vacio es false", !heap_esta_vacio(heap));

	print_test("Prueba heap encolar clave3", heap_encolar(heap, clave3));
	print_test("Prueba heap la cantidad de elementos es 3", heap_cantidad(heap) == 3);
	print_test("Prueba heap ver max es clave3, es mayor a las claves guardadas", heap_ver_max(heap) == clave3);
	print_test("Prueba heap ver max es clave3, es mayor a las claves guardadas", heap_ver_max(heap) == clave3);
	print_test("Prueba heap esta vacio es false", !heap_esta_vacio(heap));

	print_test("Prueba heap encolar clave4", heap_encolar(heap, clave4));
	print_test("Prueba heap la cantidad de elementos es 4", heap_cantidad(heap) == 4);
	print_test("Prueba heap ver max es clave3, es mayor a las claves guardadas", heap_ver_max(heap) == clave3);
	print_test("Prueba heap ver max es clave3, es mayor a las claves guardadas", heap_ver_max(heap) == clave3);
	print_test("Prueba heap esta vacio es false", !heap_esta_vacio(heap));

	print_test("Prueba heap encolar clave5", heap_encolar(heap, clave5));
	print_test("Prueba heap la cantidad de elementos es 5", heap_cantidad(heap) == 5);
	print_test("Prueba heap ver max es clave3, es mayor a las claves guardadas", heap_ver_max(heap) == clave3);
	print_test("Prueba heap ver max es clave3, es mayor a las claves guardadas", heap_ver_max(heap) == clave3);
	print_test("Prueba heap esta vacio es false", !heap_esta_vacio(heap));

	print_test("Prueba heap encolar clave6", heap_encolar(heap, clave6));
	print_test("Prueba heap la cantidad de elementos es 6", heap_cantidad(heap) == 6);
	print_test("Prueba heap ver max es clave6, es mayor a las claves guardadas", heap_ver_max(heap) == clave6);
	print_test("Prueba heap ver max es clave6, es mayor a las claves guardadas", heap_ver_max(heap) == clave6);
	print_test("Prueba heap esta vacio es false", !heap_esta_vacio(heap));


	heap_destruir(heap, NULL);
}

void prueba_heap_desencolar(void)
{
	heap_t* heap = heap_crear((cmp_func_t)strcmp);

	char* clave1 = "05";
	char* clave2 = "03";
	char* clave3 = "80";
	char* clave4 = "50";
	char* clave5 = "01";
	char* clave6 = "90";

	/* Encola 6 claves y luego las desencola. */
	print_test("Prueba heap encolar clave1", heap_encolar(heap, clave1));
	print_test("Prueba heap encolar clave2", heap_encolar(heap, clave2));
	print_test("Prueba heap encolar clave3", heap_encolar(heap, clave3));
	print_test("Prueba heap encolar clave4", heap_encolar(heap, clave4));
	print_test("Prueba heap encolar clave5", heap_encolar(heap, clave5));
	print_test("Prueba heap encolar clave6", heap_encolar(heap, clave6));

	/* Al desencolar cada elemento comprueba que los devuelve de mayor a menor. */
	print_test("Prueba heap ver max es clave6", heap_ver_max(heap) == clave6);
	print_test("Prueba heap desencolar, es clave6", heap_desencolar(heap) == clave6);
	print_test("Prueba heap la cantidad de elementos es 5", heap_cantidad(heap) == 5);

	print_test("Prueba heap ver max es clave3", heap_ver_max(heap) == clave3);
	print_test("Prueba heap desencolar, es clave3", heap_desencolar(heap) == clave3);
	print_test("Prueba heap la cantidad de elementos es 4", heap_cantidad(heap) == 4);

	print_test("Prueba heap ver max es clave4", heap_ver_max(heap) == clave4);
	print_test("Prueba heap desencolar, es clave4", heap_desencolar(heap) == clave4);
	print_test("Prueba heap la cantidad de elementos es 3", heap_cantidad(heap) == 3);

	print_test("Prueba heap ver max es clave1", heap_ver_max(heap) == clave1);
	print_test("Prueba heap desencolar, es clave1", heap_desencolar(heap) == clave1);
	print_test("Prueba heap la cantidad de elementos es 2", heap_cantidad(heap) == 2);

	print_test("Prueba heap ver max es clave2", heap_ver_max(heap) == clave2);
	print_test("Prueba heap desencolar, es clave2", heap_desencolar(heap) == clave2);
	print_test("Prueba heap la cantidad de elementos es 1", heap_cantidad(heap) == 1);

	print_test("Prueba heap ver max es clave5", heap_ver_max(heap) == clave5);
	print_test("Prueba heap desencolar, es clave5", heap_desencolar(heap) == clave5);
	print_test("Prueba heap la cantidad de elementos es 0", heap_cantidad(heap) == 0);

	print_test("Prueba heap esta vacio es true", heap_esta_vacio(heap));

	heap_destruir(heap, NULL);
}

void prueba_heap_encolar_con_destruir(void)
{
	heap_t* heap = heap_crear( (cmp_func_t) strcmp);

	char* clave1 = malloc(2 * sizeof(char));
	char* clave2 = malloc(2 * sizeof(char));
	char* clave3 = malloc(2 * sizeof(char));
	char* clave4 = malloc(2 * sizeof(char));
	char* clave5 = malloc(2 * sizeof(char));
	char* clave6 = malloc(2 * sizeof(char));
	*clave1 = '2';
	*clave2 = '5';
	*clave3 = '3';
	*clave4 = '9';
	*clave5 = '0';
	*clave6 = '1';

	print_test("Prueba heap encolar clave1", heap_encolar(heap, clave1));
	print_test("Prueba heap la cantidad de elementos es 1", heap_cantidad(heap) == 1);
	print_test("Prueba heap ver max es clave1", heap_ver_max(heap) == clave1);
	print_test("Prueba heap ver max es clave1", heap_ver_max(heap) == clave1);

	print_test("Prueba heap encolar clave2", heap_encolar(heap, clave2));
	print_test("Prueba heap la cantidad de elementos es 2", heap_cantidad(heap) == 2);
	print_test("Prueba heap ver max es clave2", heap_ver_max(heap) == clave2);
	print_test("Prueba heap ver max es clave2", heap_ver_max(heap) == clave2);

	print_test("Prueba heap encolar clave3", heap_encolar(heap, clave3));
	print_test("Prueba heap la cantidad de elementos es 3", heap_cantidad(heap) == 3);
	print_test("Prueba heap ver max es clave2", heap_ver_max(heap) == clave2);
	print_test("Prueba heap ver max es clave3", heap_ver_max(heap) == clave2);


	print_test("Prueba heap encolar clave4", heap_encolar(heap, clave4));
	print_test("Prueba heap la cantidad de elementos es 4", heap_cantidad(heap) == 4);
	print_test("Prueba heap ver max es clave4", heap_ver_max(heap) == clave4);
	print_test("Prueba heap ver max es clave4", heap_ver_max(heap) == clave4);


	print_test("Prueba heap encolar clave5", heap_encolar(heap, clave5));
	print_test("Prueba heap la cantidad de elementos es 5", heap_cantidad(heap) == 5);
	print_test("Prueba heap ver max es clave4", heap_ver_max(heap) == clave4);
	print_test("Prueba heap ver max es clave4", heap_ver_max(heap) == clave4);


	print_test("Prueba heap encolar clave6", heap_encolar(heap, clave6));
	print_test("Prueba heap la cantidad de elementos es 6", heap_cantidad(heap) == 6);
	print_test("Prueba heap ver max es clave4", heap_ver_max(heap) == clave4);
	print_test("Prueba heap ver max es clave4", heap_ver_max(heap) == clave4);

	heap_destruir(heap, free);
}

void prueba_heapsort(void)
{
	char* claves[10] = {"050","080","080","000","003","020","030","045","008","100"};

	heapsort((void*) claves, 10, (cmp_func_t) strcmp);

	/* Comparamos cada valor con el siguiente valor, se recorre hasta el anteultimo subindice del arreglo */
	int exitos = 0;
	for(int i = 0; i < 9; i++)
		if(strcmp(claves[i], claves[i+1]) <= 0 ){
			exitos++;
		}
	print_test("Prueba heapsort ordenó el arreglo", exitos == 9);
}

void prueba_heap_volumen(void)
{
	size_t len = 10000;
	int **valores = malloc(len * sizeof(int*));
	int exitos = 0;
	heap_t* heap = heap_crear((cmp_func_t) cmp);

	for (int i = 0; i < len; i++){
		valores[i] = malloc(sizeof(int));
		*valores[i] = rand();
		if(heap_encolar(heap, valores[i]))
			exitos++;
	}
	print_test("Prueba heap volumen guardar", exitos == len);
	
	heapsort((void*) valores, len,(cmp_func_t) cmp );

	exitos = 0;
	for (int i = len-1; i >= 0; i--){
		if(heap_desencolar(heap) == valores[i]){
			free(valores[i]);
			exitos++;
		}
	}
	print_test("Prueba heap volumen desencolar", exitos == len);

	free(valores);
	heap_destruir(heap, NULL);
}

/* ******************************************************************
 *                        PROGRAMA PRINCIPAL
 * *****************************************************************/

/* Programa principal. */
int main(void)
{
	prueba_heap_crear_vacio();
	prueba_heap_encolar();
	prueba_heap_desencolar();
	prueba_heap_encolar_con_destruir();
	prueba_heapsort();
	prueba_heap_volumen();

	return 0;
}
