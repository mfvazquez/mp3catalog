/*
 * prueba_AVL.c
 *
 *  Created on: 8/11/2011
 *      Author: Matías Vázquez
 *      Padron: 91523
 *      Professor: Maximiliano Curia
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "avl.h"
#include "pila.h"

/* ******************************************************************
 *                      FUNCIONES AUXILIARES
 * *****************************************************************/

/* Función auxiliar para imprimir si estuvo OK o no. */
void print_test(char* name, bool result)
{
    printf("%s: %s\n", name, result? "OK" : "ERROR");
}

/* ******************************************************************
 *                        PRUEBAS UNITARIAS
 * *****************************************************************/

/* Prueba que las primitivas funcionen correctamente. */
void prueba_crear_avl_vacio(void)
{
	avl_t* arbol = avl_crear(NULL, NULL);

	print_test("Prueba avl crear arbol vacio", arbol);
	print_test("Prueba avl la cantidad de elementos es 0", avl_cantidad(arbol) == 0);
	print_test("Prueba avl obtener clave A, es NULL, no existe", !avl_obtener(arbol, "A"));
	print_test("Prueba avl pertenece clave A, es false, no existe", !avl_pertenece(arbol, "A"));
	print_test("Prueba avl borrar clave A, es NULL, no existe", !avl_borrar(arbol, "A"));

	avl_destruir(arbol);
}

void prueba_avl_guardar(void)
{
	avl_t* arbol = avl_crear(strcmp, NULL);

	char *clave0 = "00", *valor0 = "-00-";
	char *clave1 = "10", *valor1 = "-20-";
	char *clave2 = "11", *valor2 = "-10-";
	char *clave3 = "12", *valor3 = "-22-";
	char *clave4 = "13", *valor4 = "-15-";
	char *clave5 = "14", *valor5 = "-30-";
	char *clave6 = "15", *valor6 = "-25-";
	char *clave7 = "16", *valor7 = "-35-";

	/* Inserta 1 valor y luego lo borra */
	print_test("Prueba avl insertar clave0", avl_guardar(arbol, clave0, valor0));
	print_test("Prueba avl la cantidad de elementos es 0", avl_cantidad(arbol) == 1);
	print_test("Prueba avl obtener clave0 es valor0", avl_obtener(arbol, clave0) == valor0);
	print_test("Prueba avl obtener clave0 es valor0", avl_obtener(arbol, clave0) == valor0);
	print_test("Prueba avl pertenece clave0, es true", avl_pertenece(arbol, clave0));
	print_test("Prueba avl borrar clave0, es valor0", avl_borrar(arbol, clave0) == valor0);
	print_test("Prueba avl la cantidad de elementos es 0", avl_cantidad(arbol) == 0);

	/* Inserta otros valores y no los borra */
	print_test("Prueba avl insertar clave1", avl_guardar(arbol, clave1, valor1));
	print_test("Prueba avl la cantidad de elementos es 1", avl_cantidad(arbol) == 1);
	print_test("Prueba avl obtener clave1 es valor1", avl_obtener(arbol, clave1) == valor1);
	print_test("Prueba avl obtener clave1 es valor1", avl_obtener(arbol, clave1) == valor1);
	print_test("Prueba avl pertenece clave1, es true", avl_pertenece(arbol, clave1));
	
	print_test("Prueba avl insertar clave2", avl_guardar(arbol, clave2, valor2));
	print_test("Prueba avl la cantidad de elementos es 2", avl_cantidad(arbol) == 2);
	print_test("Prueba avl obtener clave2 es valor2", avl_obtener(arbol, clave2) == valor2);
	print_test("Prueba avl obtener clave2 es valor2", avl_obtener(arbol, clave2) == valor2);
	print_test("Prueba avl pertenece clave2, es true", avl_pertenece(arbol, clave2));

	print_test("Prueba avl insertar clave3", avl_guardar(arbol, clave3, valor3));
	print_test("Prueba avl la cantidad de elementos es 3", avl_cantidad(arbol) == 3);
	print_test("Prueba avl obtener clave3 es valor3", avl_obtener(arbol, clave3) == valor3);
	print_test("Prueba avl obtener clave3 es valor3", avl_obtener(arbol, clave3) == valor3);
	print_test("Prueba avl pertenece clave3, es true", avl_pertenece(arbol, clave3));

	print_test("Prueba avl insertar clave4", avl_guardar(arbol, clave4, valor4));
	print_test("Prueba avl la cantidad de elementos es 4", avl_cantidad(arbol) == 4);
	print_test("Prueba avl obtener clave4 es valor4", avl_obtener(arbol, clave4) == valor4);
	print_test("Prueba avl obtener clave4 es valor4", avl_obtener(arbol, clave4) == valor4);
	print_test("Prueba avl pertenece clave4, es true", avl_pertenece(arbol, clave4));

	print_test("Prueba avl insertar clave5", avl_guardar(arbol, clave5, valor5));
	print_test("Prueba avl la cantidad de elementos es 5", avl_cantidad(arbol) == 5);
	print_test("Prueba avl obtener clave5 es valor5", avl_obtener(arbol, clave5) == valor5);
	print_test("Prueba avl obtener clave5 es valor5", avl_obtener(arbol, clave5) == valor5);
	print_test("Prueba avl pertenece clave5, es true", avl_pertenece(arbol, clave5));

	print_test("Prueba avl insertar clave6", avl_guardar(arbol, clave6, valor6));
	print_test("Prueba avl la cantidad de elementos es 6", avl_cantidad(arbol) == 6);
	print_test("Prueba avl obtener clave6 es valor6", avl_obtener(arbol, clave6) == valor6);
	print_test("Prueba avl obtener clave6 es valor6", avl_obtener(arbol, clave6) == valor6);
	print_test("Prueba avl pertenece clave6, es true", avl_pertenece(arbol, clave6));

	print_test("Prueba avl insertar clave7", avl_guardar(arbol, clave7, valor7));
	print_test("Prueba avl la cantidad de elementos es 7", avl_cantidad(arbol) == 7);
	print_test("Prueba avl obtener clave7 es valor7", avl_obtener(arbol, clave7) == valor7);
	print_test("Prueba avl obtener clave7 es valor7", avl_obtener(arbol, clave7) == valor7);
	print_test("Prueba avl pertenece clave7, es true", avl_pertenece(arbol, clave7));


	avl_destruir(arbol);
}

void prueba_avl_reemplazar(void)
{
	avl_t* arbol = avl_crear(strcmp, NULL);

	char *clave1 = "20", *valor1a = "-20-", *valor1b = "-020-";
	char *clave2 = "10", *valor2a = "-10-", *valor2b = "-010-";
	char *clave3 = "05", *valor3a = "-05-", *valor3b = "-005-";
	char *clave4 = "15", *valor4a = "-15-", *valor4b = "-015-";

	/* Inserta valores y luego los reemplaza */
	print_test("Prueba avl insertar clave1", avl_guardar(arbol, clave1, valor1a));
	print_test("Prueba avl obtener clave1 es valor1a", avl_obtener(arbol, clave1) == valor1a);
	print_test("Prueba avl obtener clave1 es valor1a", avl_obtener(arbol, clave1) == valor1a);
	print_test("Prueba avl insertar clave2", avl_guardar(arbol, clave2, valor2a));
	print_test("Prueba avl obtener clave2 es valor2a", avl_obtener(arbol, clave2) == valor2a);
	print_test("Prueba avl obtener clave2 es valor2a", avl_obtener(arbol, clave2) == valor2a);
	print_test("Prueba avl insertar clave2", avl_guardar(arbol, clave3, valor3a));
	print_test("Prueba avl obtener clave2 es valor2a", avl_obtener(arbol, clave3) == valor3a);
	print_test("Prueba avl obtener clave2 es valor2a", avl_obtener(arbol, clave3) == valor3a);
	print_test("Prueba avl insertar clave2", avl_guardar(arbol, clave4, valor4a));
	print_test("Prueba avl obtener clave2 es valor2a", avl_obtener(arbol, clave4) == valor4a);
	print_test("Prueba avl obtener clave2 es valor2a", avl_obtener(arbol, clave4) == valor4a);
	print_test("Prueba avl la cantidad de elementos es 4", avl_cantidad(arbol) == 4);

	print_test("Prueba avl insertar clave1 con otro valor", avl_guardar(arbol, clave1, valor1b));
	print_test("Prueba avl obtener clave1 es valor1b", avl_obtener(arbol, clave1) == valor1b);
	print_test("Prueba avl obtener clave1 es valor1b", avl_obtener(arbol, clave1) == valor1b);
	print_test("Prueba avl insertar clave2 con otro valor", avl_guardar(arbol, clave2, valor2b));
	print_test("Prueba avl obtener clave2 es valor2b", avl_obtener(arbol, clave2) == valor2b);
	print_test("Prueba avl obtener clave2 es valor2b", avl_obtener(arbol, clave2) == valor2b);
	print_test("Prueba avl insertar clave1 con otro valor", avl_guardar(arbol, clave3, valor3b));
	print_test("Prueba avl obtener clave1 es valor1b", avl_obtener(arbol, clave3) == valor3b);
	print_test("Prueba avl obtener clave1 es valor1b", avl_obtener(arbol, clave3) == valor3b);
	print_test("Prueba avl insertar clave2 con otro valor", avl_guardar(arbol, clave4, valor4b));
	print_test("Prueba avl obtener clave2 es valor2b", avl_obtener(arbol, clave4) == valor4b);
	print_test("Prueba avl obtener clave2 es valor2b", avl_obtener(arbol, clave4) == valor4b);
	print_test("Prueba avl la cantidad de elementos es 4", avl_cantidad(arbol) == 4);

	avl_destruir(arbol);
}

void prueba_avl_reemplazar_con_destruir(void)
{
	avl_t* arbol = avl_crear(strcmp, free);

	char *clave1 = "20", *valor1a, *valor1b;
	char *clave2 = "10", *valor2a, *valor2b;
	char *clave3 = "05", *valor3a, *valor3b;
	char *clave4 = "15", *valor4a, *valor4b;

	/* Pide memoria para 8 valores */
	valor1a = malloc(10 * sizeof(char));
	valor1b = malloc(10 * sizeof(char));
	valor2a = malloc(10 * sizeof(char));
	valor2b = malloc(10 * sizeof(char));
	valor3a = malloc(10 * sizeof(char));
	valor3b = malloc(10 * sizeof(char));
	valor4a = malloc(10 * sizeof(char));
	valor4b = malloc(10 * sizeof(char));
		

	/* Inserta 4 valores y luego los reemplaza (debe liberar lo que reemplaza) */
	print_test("Prueba avl insertar clave1", avl_guardar(arbol, clave1, valor1a));
	print_test("Prueba avl obtener clave1 es valor1a", avl_obtener(arbol, clave1) == valor1a);
	print_test("Prueba avl obtener clave1 es valor1a", avl_obtener(arbol, clave1) == valor1a);
	print_test("Prueba avl insertar clave2", avl_guardar(arbol, clave2, valor2a));
	print_test("Prueba avl obtener clave2 es valor2a", avl_obtener(arbol, clave2) == valor2a);
	print_test("Prueba avl obtener clave2 es valor2a", avl_obtener(arbol, clave2) == valor2a);
	print_test("Prueba avl insertar clave1", avl_guardar(arbol, clave3, valor3a));
	print_test("Prueba avl obtener clave1 es valor1a", avl_obtener(arbol, clave3) == valor3a);
	print_test("Prueba avl obtener clave1 es valor1a", avl_obtener(arbol, clave3) == valor3a);
	print_test("Prueba avl insertar clave2", avl_guardar(arbol, clave4, valor4a));
	print_test("Prueba avl obtener clave2 es valor2a", avl_obtener(arbol, clave4) == valor4a);
	print_test("Prueba avl obtener clave2 es valor2a", avl_obtener(arbol, clave4) == valor4a);
	print_test("Prueba avl la cantidad de elementos es 4", avl_cantidad(arbol) == 4);
	
	print_test("Prueba avl insertar clave1 con otro valor", avl_guardar(arbol, clave1, valor1b));
	print_test("Prueba avl obtener clave1 es valor1b", avl_obtener(arbol, clave1) == valor1b);
	print_test("Prueba avl obtener clave1 es valor1b", avl_obtener(arbol, clave1) == valor1b);
	print_test("Prueba avl insertar clave2 con otro valor", avl_guardar(arbol, clave2, valor2b));
	print_test("Prueba avl obtener clave2 es valor2b", avl_obtener(arbol, clave2) == valor2b);
	print_test("Prueba avl obtener clave2 es valor2b", avl_obtener(arbol, clave2) == valor2b);
	print_test("Prueba avl insertar clave1 con otro valor", avl_guardar(arbol, clave3, valor3b));
	print_test("Prueba avl obtener clave1 es valor1b", avl_obtener(arbol, clave3) == valor3b);
	print_test("Prueba avl obtener clave1 es valor1b", avl_obtener(arbol, clave3) == valor3b);
	print_test("Prueba avl insertar clave2 con otro valor", avl_guardar(arbol, clave4, valor4b));
	print_test("Prueba avl obtener clave2 es valor2b", avl_obtener(arbol, clave4) == valor4b);
	print_test("Prueba avl obtener clave2 es valor2b", avl_obtener(arbol, clave4) == valor4b);
	print_test("Prueba avl la cantidad de elementos es 4", avl_cantidad(arbol) == 4);

	/* Se destruye el avl (se debe liberar lo que quedó dentro) */
	avl_destruir(arbol);
}

void prueba_avl_borrar(void)
{
	avl_t* arbol = avl_crear(strcmp, NULL);

	char *clave1 = "20", *valor1 = "-20-";
	char *clave2 = "10", *valor2 = "-10-";
	char *clave3 = "05", *valor3 = "-05-";
	char *clave4 = "15", *valor4 = "-15-";

	/* Inserta 3 valores y luego los borra */
	print_test("Prueba avl insertar clave1", avl_guardar(arbol, clave1, valor1));
	print_test("Prueba avl insertar clave2", avl_guardar(arbol, clave2, valor2));
	print_test("Prueba avl insertar clave3", avl_guardar(arbol, clave3, valor3));
	print_test("Prueba avl insertar clave4", avl_guardar(arbol, clave4, valor4));

	/* Al borrar cada elemento comprueba que ya no está pero los otros sí. */
	print_test("Prueba avl pertenece clave1, es verdadero", avl_pertenece(arbol, clave1));
	print_test("Prueba avl borrar clave1, es valor1", avl_borrar(arbol, clave1) == valor1);
	print_test("Prueba avl borrar clave1, es NULL", !avl_borrar(arbol, clave1));
	print_test("Prueba avl pertenece clave1, es falso", !avl_pertenece(arbol, clave1));
	print_test("Prueba avl obtener clave1, es NULL", !avl_obtener(arbol, clave1));
	print_test("Prueba avl la cantidad de elementos es 3", avl_cantidad(arbol) == 3);

	print_test("Prueba avl pertenece clave2, es verdadero", avl_pertenece(arbol, clave2));
	print_test("Prueba avl borrar clave2, es valor2", avl_borrar(arbol, clave2) == valor2);
	print_test("Prueba avl borrar clave2, es NULL", !avl_borrar(arbol, clave2));
	print_test("Prueba avl pertenece clave2, es falso", !avl_pertenece(arbol, clave2));
	print_test("Prueba avl obtener clave2, es NULL", !avl_obtener(arbol, clave2));
	print_test("Prueba avl la cantidad de elementos es 2", avl_cantidad(arbol) == 2);

	print_test("Prueba avl pertenece clave3, es verdadero", avl_pertenece(arbol, clave3));
	print_test("Prueba avl borrar clave3, es valor3", avl_borrar(arbol, clave3) == valor3);
	print_test("Prueba avl borrar clave3, es NULL", !avl_borrar(arbol, clave3));
	print_test("Prueba avl pertenece clave3, es falso", !avl_pertenece(arbol, clave3));
	print_test("Prueba avl obtener clave3, es NULL", !avl_obtener(arbol, clave3));
	print_test("Prueba avl la cantidad de elementos es 1", avl_cantidad(arbol) == 1);

	print_test("Prueba avl pertenece clave4, es verdadero", avl_pertenece(arbol, clave4));
	print_test("Prueba avl borrar clave4, es valor4", avl_borrar(arbol, clave4) == valor4);
	print_test("Prueba avl borrar clave4, es NULL", !avl_borrar(arbol, clave4));
	print_test("Prueba avl pertenece clave4, es falso", !avl_pertenece(arbol, clave4));
	print_test("Prueba avl obtener clave4, es NULL", !avl_obtener(arbol, clave4));
	print_test("Prueba avl la cantidad de elementos es 0", avl_cantidad(arbol) == 0);

	avl_destruir(arbol);
}

void prueba_iterar_avl_vacio(void)
{
	avl_t* arbol = avl_crear(strcmp,NULL);
	avl_iter_t* iter = avl_iter_in_crear(arbol);
	print_test("Prueba avl iter in crear iterador arbol vacio", iter);
	print_test("Prueba avl iter esta al final", avl_iter_in_al_final(iter));
	print_test("Prueba avl iter avanzar es false", !avl_iter_in_avanzar(iter));
	print_test("Prueba avl iter ver actual es NULL", !avl_iter_in_ver_actual(iter));

	avl_iter_in_destruir(iter);
	avl_destruir(arbol);
}

bool guardar(const char* clave, void* dato, void* pila)
{
	return pila_apilar(pila, dato);
}

void prueba_avl_iterar(void)
{
	avl_t* arbol = avl_crear(strcmp, NULL);
	int exitos = 0;
	char *claves[14] = {"25","15","30","10","20","28","35","05","13","22","32","40","50","45"};
	const char *anterior, *actual;
	for(int i = 0; i < 14; i++)
		if (avl_guardar(arbol, claves[i], claves[i]))
			exitos++;

	print_test("Prueba avl insertar 14 elementos", exitos == 14);

	avl_iter_t* iter = avl_iter_in_crear(arbol);
	
	print_test("Prueba avl iterador esta al final, es false", !avl_iter_in_al_final(iter));
	
	/* Creamos una pila y almacenamos los datos en inorder, deben ir desapilandose de mayor a menor */
	pila_t* pila = pila_crear();
	avl_in_order(arbol, guardar, pila);
	exitos = 0;
	
	anterior = pila_desapilar(pila);
	while(!pila_esta_vacia(pila)){
		actual = pila_desapilar(pila);
		if(strcmp(anterior, actual) > 0)
			exitos++;
	}
	print_test("Prueba avl in order", exitos == 13);
	pila_destruir(pila);
	
	anterior = avl_iter_in_ver_actual(iter);
	print_test("Prueba iter avanzar es true", avl_iter_in_avanzar(iter));
	for (int i = 0, exitos = 0; i < 13; i++){
		actual = avl_iter_in_ver_actual(iter);
		if (strcmp(anterior, actual) < 0){
			exitos++;
			avl_iter_in_avanzar(iter);
		}
	}
	print_test("Prueba iter recorre ordenado", exitos == 13);
	print_test("Prueba iter al final es true", avl_iter_in_al_final(iter));
	print_test("Prueba iter avanzar es false", !avl_iter_in_avanzar(iter));
	
	avl_iter_in_destruir(iter);
	avl_destruir(arbol);

}

void prueba_avl_borrar2(void)
{
	avl_t* arbol = avl_crear(strcmp, NULL);
	char *claves[14] = {"05","02","08","01","07","03","10","06","12","13","15","09","04","99"};
	int *valores[14];
	printf("Se guardaran 14 elementos en el arbol\n");
	int exitos = 0;
	for (int i = 0; i < 14; i++){
		valores[i] = malloc(sizeof(int));
		if (avl_guardar(arbol, claves[i], valores[i])) exitos++;
	}

	print_test("Prueba de guardar 14 elementos", exitos == 14);

	for (int i = 13, exitos = 0; i >= 0; i--){
		if (avl_cantidad(arbol) == i && avl_borrar(arbol, claves[i])) exitos++;
		free(valores[i]);
	}

	print_test("Prueba de borrar 14 elementos", exitos == 14);

	avl_destruir(arbol);

}

void prueba_avl_volumen(void)
{
	avl_t* arbol = avl_crear(strcmp,free);

	size_t largo = 5000;
	const size_t largo_clave = 5;
	char (*claves)[largo_clave] = malloc(largo * largo_clave);

	int* valores[largo];

	bool ok = true;
	for (int i = 0; i < largo; i++) {
		valores[i] = malloc(sizeof(int));
		sprintf(claves[i], "%04d", i);
		*valores[i] = i;
		ok = avl_guardar(arbol, claves[i], valores[i]);
		if (!ok) break;
	}
	printf("Prueba avl volumen\n");
	printf("cantidad de datos = %lu\n", avl_cantidad(arbol));
	printf("altura del arbol = %lu\n", avl_altura(arbol));

	int pendiente;
	avl_iter_t* iter = avl_iter_in_crear(arbol);
	bool exito = true;
	while (!avl_iter_in_al_final(iter)){
		pendiente = avl_iter_in_ver_pendiente(iter);
		if (pendiente > 1 || pendiente < -1)
			exito = false;
		avl_iter_in_avanzar(iter);
	}
	avl_iter_in_destruir(iter);
	print_test("Prueba avl volumen pendientes correctas", exito);

	exito = true;
	for (int i = 0; i < largo; i++) {
		if (avl_borrar(arbol,claves[i]) != valores[i])
			exito = false;
		free(valores[i]);
	}
	print_test("Prueba avl volumen, borrar todos los datos", exito);

	free(claves);
	avl_destruir(arbol);
}

void prueba_avl_volumen_random(void)
{
	avl_t* arbol = avl_crear(strcmp,free);

	size_t largo = 5000;
	const size_t largo_clave = 13;
	char (*claves)[largo_clave] = malloc(largo * largo_clave);

	int* valores[largo];

	bool ok = true;
	for (int i = 0; i < largo; i++) {
		valores[i] = malloc(sizeof(int));
		sprintf(claves[i], "%012d", rand());
		*valores[i] = i;
		ok = avl_guardar(arbol, claves[i], valores[i]);
		if (!ok) break;
	}
	printf("Prueba avl volumen random\n");
	printf("cantidad de datos = %lu\n", avl_cantidad(arbol));
	printf("altura del arbol = %lu\n", avl_altura(arbol));

	int pendiente;
	avl_iter_t* iter = avl_iter_in_crear(arbol);
	bool exito = true;
	while (!avl_iter_in_al_final(iter)){
		pendiente = avl_iter_in_ver_pendiente(iter);
		if (pendiente > 1 || pendiente < -1)
			exito = false;
		avl_iter_in_avanzar(iter);
	}
	avl_iter_in_destruir(iter);
	print_test("Prueba avl volumen con claves random pendientes correctas", exito);

	free(claves);
	avl_destruir(arbol);
}

void prueba_avl_volumen2(size_t largo, bool debug)
{
	avl_t* arbol = avl_crear(strcmp, NULL);

	const size_t largo_clave = 10;
	char (*claves)[largo_clave] = malloc(largo * largo_clave);

	int* valores[largo];

	/* Inserta 'largo' parejas en el arbol */
	bool ok = true;
	for (int i = 0; i < largo; i++) {
		valores[i] = malloc(sizeof(int));
		sprintf(claves[i], "%08d", i);
		*valores[i] = i;
		ok = avl_guardar(arbol, claves[i], valores[i]);
		if (!ok) break;
	}

	if (debug) print_test("Prueba avl almacenar muchos elementos", ok);
	if (debug) print_test("Prueba avl la cantidad de elementos es correcta", avl_cantidad(arbol) == largo);

	/* Verifica que devuelva los valores correctos */
	for (size_t i = 0; i < largo; i++) {
		ok = avl_pertenece(arbol, claves[i]);
		if (!ok) break;
		ok = avl_obtener(arbol, claves[i]) == valores[i];
		if (!ok) break;
	}

	if (debug) print_test("Prueba avl pertenece y obtener muchos elementos", ok);
	if (debug) print_test("Prueba avl la cantidad de elementos es correcta", avl_cantidad(arbol) == largo);

	/* Verifica que borre y devuelva los valores correctos */
	for (size_t i = 0; i < largo; i++) {
		ok = (avl_borrar(arbol, claves[i]) == valores[i]);
		if (!ok) break;
	}

	if (debug) print_test("Prueba avl borrar muchos elementos", ok);
	if (debug) print_test("Prueba avl la cantidad de elementos es 0", avl_cantidad(arbol) == 0);

	/* Destruye el arbol y crea uno nuevo que sí libera */
	avl_destruir(arbol);
	arbol = avl_crear(strcmp, free);

	/* Inserta 'largo' parejas en el avl */
	ok = true;
	for (size_t i = 0; i < largo; i++) {
		ok = avl_guardar(arbol, claves[i], valores[i]);
		if (!ok) break;
	}

	/* Destruye el avl - debería liberar los enteros */
	avl_destruir(arbol);
	free(claves);
}



/* ******************************************************************
 *                        PROGRAMA PRINCIPAL
 * *****************************************************************/

/* Programa principal. */
int main(int argc, char** argv)
{
	if (argc < 2){
		prueba_crear_avl_vacio();
		prueba_iterar_avl_vacio();
		prueba_avl_guardar();
		prueba_avl_borrar();
		prueba_avl_reemplazar();
		prueba_avl_reemplazar_con_destruir();
		prueba_avl_iterar();
		prueba_avl_borrar2();
		prueba_avl_volumen();
		prueba_avl_volumen_random();
	} else {
		size_t largo = atoi(argv[1]);
		prueba_avl_volumen2(largo, false);
	}
	return 0;
}
