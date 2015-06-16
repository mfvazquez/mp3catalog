/*
 * avl.c
 *
 *  Created on: 8/11/2011
 *      Author: Matías Vázquez
 *      Padron: 91523
 *      Professor: Maximiliano Curia
 */

#include "avl.h"
#include <stdlib.h>
#include <string.h>

/* ******************************************************************
 *                DEFINICION DE LOS TIPOS DE DATOS
 * ******************************************************************/

typedef struct nodo{
	void* dato;
	char* clave;
	struct nodo* izq;
	struct nodo* der;
	size_t altura;
}nodo_t;

struct avl{
	nodo_t* raiz;
	size_t cantidad;
	avl_comparar_clave_t cmp;
	avl_destruir_dato_t destruir_dato;
};

struct avl_iter{
	pila_t* pila;
};

#define max(A, B) ((A) > (B) ? (A) : (B))

/* ******************************************************************
 * 					FUNCIONES AUXILIARES
 * ******************************************************************/

/*
 * Crea el nodo, guardando sus datos, y hace una copia de la clave.
 * Pre: ~
 * Post: Se devolvió un puntero a nodo.
 */
nodo_t* nodo_crear(const char* clave, void* dato)
{
	size_t len = strlen(clave);

	nodo_t* nodo = malloc(sizeof(nodo_t));
	if (!nodo) return NULL;

	nodo->clave = malloc( (len + 1) * sizeof(char) );
	if ( !nodo->clave ){
		free(nodo);
		return NULL;
	}

	for (int i = 0; i < len; i++)
		nodo->clave[i] = clave[i];
	nodo->clave[len] = '\0';
	nodo->dato = dato;
	nodo->izq = NULL;
	nodo->der = NULL;
	nodo->altura = 1;

	return nodo;
}

/*
 * Libera el espacio de memoria recervado por el nodo, y devuelve
 * su dato
 * Pre: EL nodo fué creado.
 * Post: Se liberó espacio en memoria y se devolvió el dato.
 */
void* nodo_destruir(nodo_t* nodo)
{
	void* dato = nodo->dato;
	free(nodo->clave);
	free(nodo);
	return dato;
}

/*
 * Funcion que recorre el avl, en busca del nodo con la clave ingresada,
 * y asigna ese nodo al puntero actual, y guardara en actual_cmp la
 * comparación de la clave del nodo actual y la clave ingresada.
 * De ser distintos de NULL, anterior apuntará al nodo anterior a actual,
 * y guardará en anterior_cmp la comparación de la clave del nodo anterior
 * y la clave ingresada.
 * Pre: el arbol fue creado, los punteros actual y actual_cmp deben estar
 * creados.
 * Post: el puntero actual apunta a un nodo y actual_cmp guardó el valor
 * de la comparación, tambien se guardó el nodo anterior y la comparación
 * anterior en caso de ser requerido.
 */
void nodo_obtener(const avl_t* arbol, const char* clave, nodo_t** actual, int* actual_cmp)
{
	if (!arbol->raiz){
		*actual = NULL;
		return;
	}
	*actual = arbol->raiz;
	*actual_cmp = arbol->cmp((*actual)->clave, clave);
	while ((*actual_cmp > 0 && (*actual)->izq) || (*actual_cmp < 0 && (*actual)->der)){
		if ( *actual_cmp > 0 ) *actual = (*actual)->izq;
		else if(*actual_cmp < 0 ) *actual = (*actual)->der;

		*actual_cmp = arbol->cmp((*actual)->clave, clave);
	}
}

/*
 * Recorre el arbol en busca del nodo con la clave ingresada y apila los nodos
 * a medida que recorre el arbol, hasta llegar al nodo con la clave o hasta
 * que no pueda seguir recorriendo.
 * Pre: el arbol y la pila fueron creados.
 * Post: se apilaron nodos en la pila.
 */
void apilar_recorrido(const avl_t* arbol, const char* clave, pila_t* pila)
{
	if (arbol->raiz == NULL)
		return;

	nodo_t* actual = arbol->raiz;
	int actual_cmp = arbol->cmp(actual->clave, clave);
	pila_apilar(pila, actual);
	while ((actual_cmp > 0 && actual->izq) || (actual_cmp < 0 && actual->der)){
		if (actual_cmp > 0){
			actual = actual->izq;
		}else if (actual_cmp < 0){
			actual = actual->der;
		}
		actual_cmp = arbol->cmp(actual->clave, clave);
		pila_apilar(pila, actual);
	}
}

/*
 * Funcion recursiva que recorre los nodos en preorder y le aplica la
 * funcion a los datos.
 * Pre: el nodo fue creado.
 * Post: Se destruyeron todos los nodos y datos.
 */
void destruir_nodos_postorder(nodo_t* nodo, avl_destruir_dato_t destruir_dato)
{
	if(!nodo) return;
	destruir_nodos_postorder(nodo->izq, destruir_dato);
	destruir_nodos_postorder(nodo->der, destruir_dato);
	void* dato = nodo_destruir(nodo);
	if (destruir_dato) destruir_dato(dato);
}

/*
 * Funcion recursiva que recorre los nodos en inorder y le aplica la
 * funcion a los nodos.
 * Pre: El nodo fue creado y la funcion declarada.
 * Post: Se aplico la funcion a cada nodo del arbol.
 */
void nodos_recorrer_inorder(nodo_t* nodo, bool funcion(const char *, void *, void *), void* extra)
{
	if (!nodo) return;
	nodos_recorrer_inorder(nodo->izq, funcion, extra);
	funcion(nodo->clave, nodo->dato, extra);
	nodos_recorrer_inorder(nodo->der, funcion, extra);
}

/*
 * Calcula la pendiente del nodo recibido.
 * cuyo calculo es:
 *
 * 	pendiente = nodo->izq->altura - nodo->der->altura
 *
 * Pre: el nodo fue creado.
 * Post: se devolvió la pendiente del nodo
 */
int pendiente(nodo_t* nodo)
{
	int altura_der = 0, altura_izq = 0;
	if(nodo->der)
		altura_der = nodo->der->altura;
	if(nodo->izq)
		altura_izq = nodo->izq->altura;
	return altura_izq - altura_der;
}

/*
 * Actualiza la altura del nodo recibido, que es igual a la altura maxima
 * de alguno de sus hijos mas uno.
 * Pre: el nodo fue creado.
 * Post: se actualizo la altura del nodo
 */
void actualizar_altura(nodo_t* nodo)
{
	if (nodo->izq && nodo->der)
		nodo->altura = max(nodo->izq->altura, nodo->der->altura) + 1;
	else if(nodo->izq) nodo->altura = nodo->izq->altura + 1;
	else if(nodo->der) nodo->altura = nodo->der->altura + 1;
	else nodo->altura = 1;
}

/*
 * Rota el nodo recibido a la izquierda, aumentando así la altura
 * de los nodos izquierdos y disminuyendo la altura de los nodos derechos del arbol.
 * Pre: el nodo fue creado.
 * Post: ahora nodo->der ocupa la posición del nodo en el arbol
 * y nodo esta a la izquierda de nodo->der
 */
nodo_t* rotar_izquierda(nodo_t* nodo)
{
	nodo_t* nodo_derecho = nodo->der;
	nodo->der = nodo_derecho->izq;
	nodo_derecho->izq = nodo;
	actualizar_altura(nodo_derecho->izq);
	actualizar_altura(nodo_derecho);
	return nodo_derecho;
}

/*
 * Rota el nodo recibido a la derecha, aumentando así la altura
 * de los nodos derechos y disminuyendo la altura de los nodos izquierdos del arbol.
 * Pre: el nodo fue creado.
 * Post: ahora nodo->izq ocupa la posición del nodo en el arbol
 * y nodo esta a la derecha de nodo->izq
 */
nodo_t* rotar_derecha(nodo_t* nodo)
{
	nodo_t* nodo_izquierdo = nodo->izq;
	nodo->izq = nodo_izquierdo->der;
	nodo_izquierdo->der = nodo;
	actualizar_altura(nodo_izquierdo->der);
	actualizar_altura(nodo_izquierdo);
	return nodo_izquierdo;
}

/*
 * Verifica que los nodos de la pila sigan cumpliendo la condicion de AVL
 * Pre: la pila y el avl fueron creados
 * Post: la pila quedó vacía y se realizaron las correspondientes modificaciones
 * de alturas a los nodos de la pila, y se realizaron rotaciones
 */
void actualizar_avl(avl_t* arbol, pila_t* recorrido)
{
	nodo_t* actual;
	while(!pila_esta_vacia(recorrido)){
		actual = pila_desapilar(recorrido);
		actualizar_altura(actual);
		if (pendiente(actual) == 2){
			if (pendiente(actual->izq) == -1)
				actual->izq = rotar_izquierda(actual->izq);
			actual = rotar_derecha(actual);
		}
		else if(pendiente(actual) == -2){
			if (pendiente(actual->der) == 1)
				actual->der = rotar_derecha(actual->der);
			actual = rotar_izquierda(actual);
		}
		nodo_t* anterior_actual = pila_ver_tope(recorrido);
		if(anterior_actual){
			int cmp = arbol->cmp(anterior_actual->clave, actual->clave);
			if (cmp < 0) anterior_actual->der = actual;
			else anterior_actual->izq = actual;
		}
	}
	arbol->raiz = actual;
}

/* ******************************************************************
 *                 PRIMITIVAS  BASICAS DEL AVL
 * ******************************************************************/

/*
 * Crea un avl y guarda las funciones cmp y destruir_dato.
 * Pre: cmp y destruir_dato deben estar declaradas.
 * Post: se devolvió un avl y se guardaron las funciones.
 */
avl_t* avl_crear(avl_comparar_clave_t cmp, avl_destruir_dato_t destruir_dato)
{
	avl_t* arbol = malloc(sizeof(avl_t));
	if (!arbol) return NULL;

	arbol->raiz = NULL;
	arbol->cantidad = 0;
	arbol->cmp = cmp;
	arbol->destruir_dato = destruir_dato;

	return arbol;
}

/*
 * Guarda el dato ingresado en el avl y devuelve true. En caso de error
 * devuelve false.
 * Pre: El avl debe estar creado.
 * Post: Se guardó un dato en el avl y se devolvió true, o false en caso de error.
 */
bool avl_guardar(avl_t *arbol, const char *clave, void *dato)
{
	pila_t* pila = pila_crear();
	apilar_recorrido(arbol, clave, pila);
	nodo_t* nodo = pila_ver_tope(pila); // es el nodo donde se reemplazará el dato o bien es el padre del nuevo nodo
	int cmp;

	if (nodo && (cmp = arbol->cmp(nodo->clave, clave)) == 0){ // la clave ya existe, entonces no hay necesidad de rotar/modificar alturas.
		if (arbol->destruir_dato) arbol->destruir_dato(nodo->dato);
		nodo->dato = dato;
		pila_destruir(pila);
		return true;
	}

	// no existe la clave, creamos un nodo nuevo y lo guardamos
	nodo_t* nodo_nuevo = nodo_crear(clave, dato);
	if (!nodo_nuevo) return false;

	if (!nodo) arbol->raiz = nodo_nuevo; //el arbol está vacío
	else if (cmp > 0) nodo->izq = nodo_nuevo;
	else nodo->der = nodo_nuevo; //cmp < 0

	arbol->cantidad++;

	if (!pila_esta_vacia(pila)){
		actualizar_avl(arbol, pila);
	}
	pila_destruir(pila);

	return true;
}

/*
 * Borra del avl el dato correspondiente a la clave ingresada y lo devuelve.
 * En caso de no encontrarse el dato, devuelve NULL.
 * Pre: El avl fue creado.
 * Post: Se borró del avl el dato correspondiente a la clave ingresada y fue
 * devuelto, en caso de no encontrarse se devolvió NULL.
 */
void *avl_borrar(avl_t *arbol, const char *clave)
{
	pila_t* recorrido = pila_crear();
	apilar_recorrido(arbol, clave, recorrido);
	nodo_t* actual = pila_desapilar(recorrido);
	nodo_t* anterior = pila_ver_tope(recorrido);
	int cmp_actual, cmp_anterior;

	if (actual)
		cmp_actual = arbol->cmp(actual->clave, clave);
	if (anterior)
		cmp_anterior = arbol->cmp(anterior->clave, actual->clave);

	if (!actual || cmp_actual != 0){ // el arbol está vacío o no se encuentra la clave
		pila_destruir(recorrido);
		return NULL;
	}

	if (!actual->izq && !actual->der){ // el nodo no tiene hijos
		if (anterior){
			if (cmp_anterior > 0) anterior->izq = NULL;
			else anterior->der = NULL;
		}else
			arbol->raiz = NULL;

	}else if (actual->izq && actual->der){ // el nodo tiene 2 hijos
		nodo_t *mayor = actual->izq;
		nodo_t* recorrido_mayor[mayor->altura]; // vector que guarda los nodos recorridos en busca del nodo mayor
		int contador = 1;
		recorrido_mayor[0] = mayor;

		while(mayor->der){
			mayor = mayor->der;
			recorrido_mayor[contador] = mayor;
			contador++;
		}
		pila_apilar(recorrido, recorrido_mayor[--contador]);
		for (int x = 0; x < contador; x++){
			pila_apilar(recorrido, recorrido_mayor[x]);
		}

		nodo_t* ant_mayor = NULL;
		if (contador > 0) ant_mayor = recorrido_mayor[contador - 1];

		if (mayor->izq && ant_mayor) ant_mayor->der = mayor->izq;
		else if (ant_mayor) ant_mayor->der = NULL;

		mayor->der = actual->der;
		if (actual->izq != mayor) mayor->izq = actual->izq;

		if (anterior){
			if (cmp_anterior > 0) anterior->izq = mayor;
			else anterior->der = mayor;
		}else{
			arbol->raiz = mayor;
		}

	}else if (actual->izq && !actual->der){ // el nodo solo tiene un hijo izquierdo
		if (anterior){
			if (cmp_anterior < 0) anterior->der = actual->izq;
			else anterior->izq = actual->izq;
		}else arbol->raiz = actual->izq;

	}else{ // el nodo solo tiene un hijo derecho
		if(anterior){
			if (cmp_anterior < 0) anterior->der = actual->der;
			else anterior->izq = actual->der;
		}else arbol->raiz = actual->der;
	}

	arbol->cantidad--;

	if (!pila_esta_vacia(recorrido)){
		actualizar_avl(arbol, recorrido);
	}
	pila_destruir(recorrido);

	return nodo_destruir(actual);
}

/*
 * Devuelve el dato guardado en el avl correspondiente a la clave
 * ingresada. En caso de no encontrarse la clave devuelve, NULL.
 * Pre: El avl fue creado.
 * Post: Se devolvió el dato correspondiente a la clave ingresada, en caso
 * de no encontrarse se devolvió NULL.
 */
void *avl_obtener(const avl_t *arbol, const char *clave)
{
	nodo_t* nodo = NULL, **nodo_aux = &nodo;
	int cmp;
	nodo_obtener(arbol, clave, nodo_aux, &cmp);
	nodo = *nodo_aux;

	if (!nodo) return NULL; // arbol vacío

	return (cmp == 0)? nodo->dato : NULL;
}

/*
 * Devuelve true si se encuentra un dato en el avl correspondiente
 * a la clave ingresada, en caso de no encontrarse devuelve false.
 * Pre: El avl fue creado.
 * Post: Se devolvió true si se encontró un dato en el avl correspondiente
 * a la clave, caso contrario se devolvió false
 */
bool avl_pertenece(const avl_t *arbol, const char *clave)
{
	return ( avl_obtener(arbol, clave) != NULL );
}

/*
 * Devuelve la cantidad de elementos guardados en el avl.
 * Pre: El avl fue creado.
 * Post: Se devolvió la cantidad de elementos almacenados en el avl.
 */
size_t avl_cantidad(avl_t *arbol)
{
	return arbol->cantidad;
}

/*
 *
 */
size_t avl_altura(avl_t *arbol)
{
	return arbol->raiz->altura;
}

/*
 * Destruye el avl y aplica la función destruir_dato a cada dato almacenado
 * en el avl.
 * Pre: El avl fue creado.
 * Post: Se liberó el espacio ocupado en la memoria por el avl.
 */
void avl_destruir(avl_t *arbol)
{
	destruir_nodos_postorder(arbol->raiz, arbol->destruir_dato);
	free(arbol);
}

/* *****************************************************************
 *                PRIMITIVAS  BASICAS DE ITERACIÓN
 * *****************************************************************/

/*
 * Recorre el avl en inorder, aplicandole a todos los datos la funcion
 * pasada por parametro.
 * Pre: La función debe estar definida.
 * Post: Se aplico la funcion a cada dato del avl.
 */
void avl_in_order(avl_t *arbol, bool funcion(const char *, void *, void *), void *extra)
{
	nodos_recorrer_inorder(arbol->raiz, funcion, extra);
}

/*
 * Crea un iterador en el avl. Recorrerá el arbol en inorder.
 * Devuelve NULL en caso de error.
 * Pre: EL arbol fue creado.
 * Post: Se devolvió un iterador, o NULL en caso de error.
 */
avl_iter_t *avl_iter_in_crear(const avl_t *arbol)
{
	avl_iter_t* iter = malloc(sizeof(avl_iter_t));
	if (!iter) return NULL;

	iter->pila = pila_crear();
	nodo_t* nodo = arbol->raiz;
	if (nodo){ 
		pila_apilar(iter->pila, nodo);
		while(nodo->izq){
			pila_apilar(iter->pila, nodo->izq);
			nodo = nodo->izq;
		}
	}
	return iter;
}

/*
 * Avanza el iterador a la siguiente posición y devuelve true.
 * Devuelve false si ya se encuentra al final del arbol.
 * Pre: el iterador fue creado.
 * Post: Se devolvió true y el iter avanzó de posición, o bien
 * se devolvió false.
 */
bool avl_iter_in_avanzar(avl_iter_t *iter)
{
	nodo_t* nodo = pila_desapilar(iter->pila);
	if(!nodo) return false;

	if(nodo->der){
		nodo = nodo->der;
		pila_apilar(iter->pila, nodo);
		while(nodo->izq){
			nodo = nodo->izq;
			pila_apilar(iter->pila, nodo);
		}
	}
	return true;
}

/*
 * Devuelve la clave del nodo en el que esta posicionado el iterador
 * o NULL si ya se encuentra al final.
 * Pre: El iter fue creado.
 * Post: Se devolvió la clave del nodo actual o NULL si el iter está al final.
 */
const char *avl_iter_in_ver_actual(const avl_iter_t *iter)
{
	nodo_t* nodo = pila_ver_tope(iter->pila);
	if (!nodo) return NULL;
	return nodo->clave;
}

/*
 * Devuelve la pendiente del elemento actual del iter.
 * Pre: el iter fue creado.
 * Post: se devolvió la pendiente del elemento actual.
 */
int avl_iter_in_ver_pendiente(const avl_iter_t* iter)
{
	nodo_t* nodo =  pila_ver_tope(iter->pila);
	if(!nodo) return 0;
	return pendiente(nodo);
}

/*
 * Devuelve true si el iter está al final del avl, false en caso contrario.
 * Pre: El iter fue creado.
 * Post: se devolvio true o false, segun corresponda.
 */
bool avl_iter_in_al_final(const avl_iter_t *iter)
{
	return pila_esta_vacia(iter->pila);
}

/*
 * Destruye el iter, liberando el espacio de la memoria reservado.
 * Pre: El iter fue creado.
 * Post: se destruyó el iter.
 */
void avl_iter_in_destruir(avl_iter_t* iter)
{
	pila_destruir(iter->pila);
	free(iter);
}
