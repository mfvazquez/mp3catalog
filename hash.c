#include <stdlib.h>
#include <string.h>
#include "hash.h"

/* ******************************************************************
 *    			   Profesor: Maximiliano Curia
 *  Integrantes del grupo:                    Padron:
 * 		Vázquez Matías..........................91523
 *		San Pedro Martín........................91461
 * ******************************************************************/

/* ******************************************************************
 *                DEFINICION DE LOS TIPOS DE DATOS
 * ******************************************************************/

#define LONG_INI 256
#define VACIO NULL
#define BORRAR -1

typedef struct paquete{
	char* clave;
	void* dato;
}paquete_t;

struct hash{
	lista_t** vector;
	size_t tamanio;    // tamaño de vector
	size_t cantidad;   // cantidad de elementos almacenados en vector
	size_t cant_listas; // cantidad de listas en el vector
	hash_destruir_dato_t destruir_dato;
};

struct hash_iter{
	char* actual;
	size_t pos;    // posicion de hash->vector en que se encuentra el iter
	const hash_t* hash;
	lista_iter_t* iter_lista;  // iter de la lista
};

typedef void* (*paquete_destruir_t)(void*);

/* *******************************************************************
 *                      FUNCIONES AUXILIARES
 * *******************************************************************/

bool rehash(hash_t* hash, size_t tam);

/* Funcion de hashing, que recibe un arreglo de caracteres y un tope,
 * devolverá un número correspondiente a dicho arreglo de caracteres que
 * no superará el tope.
 * Pre: Se debe ingresar un arreglo de caracteres y un tope de tipo size_t
 * Post: se devolvió un número natural entre cero y tope.*/
size_t hashing(const char *clave, size_t tope){
		size_t hash, i, tam = strlen(clave);
		for(hash = i = 0; i < tam; ++i){
				hash += clave[i];
				hash += (hash << 10);
				hash ^= (hash >> 6);
		}
		hash += (hash << 3);
		hash ^= (hash >> 11);
		hash += (hash << 15);
		hash %= tope;
		return hash;
}

/* Crea un paquete, que reservará un espacio en memoria para la clave y 
 * guardara ese apuntador junto con el puntero de dato. Devuelve NULL
 * en caso de error.
 * Pre: se ingresó un arreglo de caracteres y un un puntero a algun dato
 * Post: se devolvió un paquete o NULL en caso de error.*/
static paquete_t* empaquetar(const char* clave, void* dato)
{
	size_t len = strlen(clave);
	
	paquete_t* paquete = malloc(sizeof(paquete_t));
	if (!paquete) 
		return NULL;
	char* clave_aux = malloc((len + 1) * sizeof(char));
	if( !clave_aux ){
		free(paquete);
		return NULL;
	}
	
	for(int i = 0; i < len; i++)
		clave_aux[i] = clave[i];
	clave_aux[len] = '\0';
	
	paquete->clave = clave_aux;
	paquete->dato = dato;
	
	return paquete;
}

/* Destruye el paquete y su clave, y devuelve el dato guardado en el.
 * Pre: El paquete fue creado.
 * Post: se libero el espacio reservado en la memoria por el paquete 
 * y la clave, y se devolvió el dato. */
static void* desempaquetar(paquete_t* paquete)
{
	void* dato = paquete->dato;
	free(paquete->clave);
	free(paquete);
	return dato;
}

/* Busca el paquete correspondiente a la clave y lo devuelve. 
 * Si accion es igual a BORRAR destruye el elemento de la lista donde 
 * se encuentra. Si no se encuentra un paquete asociado a dicha 
 * clave, devuelve NULL.
 * Pre: la lista fue creada.
 * Post: se devolvio el paquete correspondiente a la clave y se borro
 * el elemento de la lista en caso de ser requerido. */
paquete_t* buscar_paquete(hash_t* hash, const char* clave, int accion, size_t pos)
{
	if (hash->vector[pos] == NULL) return NULL;
	lista_iter_t* iter = lista_iter_crear(hash->vector[pos]);
	paquete_t* paquete_actual;
	
	while (!lista_iter_al_final(iter)){
		paquete_actual = lista_iter_ver_actual(iter);
		
		if (strcmp(clave, paquete_actual->clave) == 0){
			if(accion == BORRAR){
				lista_borrar(hash->vector[pos], iter);
				hash->cantidad--;
				if(lista_esta_vacia(hash->vector[pos])){
					lista_destruir(hash->vector[pos], NULL);
					hash->cant_listas--;
					hash->vector[pos] = NULL;
				}
			}
			lista_iter_destruir(iter);
			return paquete_actual;
		}
		lista_iter_avanzar(iter);
	}
	lista_iter_destruir(iter);
	return NULL;  // No se encontro la clave
}

/* Guarda en la tabla de hash el paquete ingresado
 * Pre: La tabla de hash y el paquete fueron creados
 * Post: se devuelvió true, o false en caso de error.  */
bool guardar_paquete(hash_t* hash, paquete_t* paquete)
{	
	size_t pos = hashing(paquete->clave, hash->tamanio);
	
	if (hash->vector[pos] == VACIO){
		// Si aun no se han guardado datos en esta posicion
		lista_t* lista = lista_crear();
		if (!lista) return false;
		lista_insertar_primero(lista, paquete);
		hash->vector[pos] = lista;
		hash->cant_listas++;
		hash->cantidad++;
		return true;
	}
	paquete_t* paquete_aux;
	if ((paquete_aux = buscar_paquete(hash, paquete->clave, 0, pos)) != NULL ){
		// Si ya hay una lista con un paquete con esa clave
		if (hash->destruir_dato)
			hash->destruir_dato(paquete_aux->dato);
		paquete_aux->dato = paquete->dato;
		free(paquete->clave);
		free(paquete);
		return true;
	}
	// Si ya hay una lista en esa posicion
	lista_insertar_primero(hash->vector[pos], paquete);
	hash->cantidad++;
	if (lista_largo(hash->vector[pos]) == 10)
		return rehash( hash, hash->tamanio *2 );
	return true;
}

/* Crea un nuevo vector de datos y lo reemplaza en el hash. Guarda
 * los datos del vector viejo al nuevo.
 * Pre: La tabla de hash fue creada
 * Post: se devolvió true si se logro redimensionar el vector del hash.
 * Si ocurrió un error devuelve false */
bool rehash(hash_t* hash, size_t tam)
{
	// Creamos el nuevo hash
	lista_t **vector_aux = hash->vector;
	size_t tam_aux = hash->tamanio;
	lista_t **vector_nuevo = calloc(tam , sizeof(lista_t*));
	if (!vector_nuevo ) return false;
	hash->cantidad = 0;
	hash->cant_listas = 0;
	hash->tamanio = tam;
	hash->vector = vector_nuevo;
	paquete_t* paquete;
	
	// Guardamos los datos del hash viejo al nuevo
	for( int i = 0; i < tam_aux; i++){
		while (vector_aux[i] != VACIO){
			paquete = lista_borrar_primero(vector_aux[i]);
			guardar_paquete(hash, paquete);
			if(lista_esta_vacia(vector_aux[i])){
				lista_destruir(vector_aux[i],NULL);
				vector_aux[i] = VACIO;
			}
		}
	}
	// Liberamos el espacio guardado por el hash viejo y ahora apuntamos al nuevo hash
	free(vector_aux);
	return true;
}

/* *******************************************************************
 *       IMPLEMENTACION DE FUNCIONES BASICAS DE LA TABLA DE HASH
 * **********************, void* desemnpaquetar(void*)*********************************************/

/* Crea una tabla de hash vacía y guarda la funcion destruir_dato.
 * Pre: la funcion destruir_dato ya está definida. 
 * Post: se devolvió una tabla de hash vacía o NULL en caso de error.*/
hash_t *hash_crear(hash_destruir_dato_t destruir_dato)
{	
	hash_t* hash = malloc(sizeof(hash_t));
	hash->vector = calloc(LONG_INI , sizeof(lista_t*));
	if (!hash || !hash->vector ) return NULL;
	
	hash->destruir_dato = destruir_dato;
	hash->tamanio = LONG_INI;
	hash->cantidad = 0;
	hash->cant_listas = 0;

	return hash;	
}

/* Destruye la tabla hash ingresada y aplica la funcion destruir_dato
 * a cada dato guardado en el hash, si destruir_dato es NULL solo
 * destruirá la tabla de hash.
 * Pre: la tabla de hash fué creada.
 * Post: se destruye el hash y se aplico la funcion destruir_dato
 * a cada dato del hash.*/
void hash_destruir(hash_t *hash)
{
	void *paquete = NULL;
	void* dato = NULL;
	for (int i = 0; i < hash->tamanio; i++)
		while(hash->vector[i] != VACIO){
			paquete = lista_borrar_primero(hash->vector[i]);
			dato = desempaquetar(paquete);
			if (hash->destruir_dato)
				hash->destruir_dato(dato);
			if (lista_esta_vacia(hash->vector[i])){
				lista_destruir(hash->vector[i], NULL);
				hash->vector[i] = VACIO;
			}
		}
	free (hash->vector);
	free (hash);
}

/* Agrega un nuevo elemento a la tabla de hash. Devuelve falso en caso 
 * de error.
 * Pre: la tabla de hash fué creada.
 * Post: se guardó un nuevo elemento en el hash y devolvió verdadero
 * de tener exito, sino falso.*/
bool hash_guardar(hash_t *hash, const char *clave, void *dato)
{
	paquete_t *paquete = empaquetar(clave, dato);
	return guardar_paquete(hash, paquete);
	
}
	
/* Devuelve verdadero si el elemento asociado a la clave se encuentra 
 * en la tabla de hash, caso contrario devuelve falso.
 * Pre: la tabla de hash fué creada.
 * Post: se devolvió verdadero o falso.*/
bool hash_pertenece(hash_t *hash, const char *clave)
{
	return (hash_obtener(hash, clave) != NULL);
}

/* Quita el elemento, correspondiente a la clave, almacenado en la 
 * tabla de hash, o devuelve NULL en caso de no encontrarse almacenado.
 * Pre: El hash fué creado.
 * Post: Se devolvió el dato correspondiente a la clave, o NULL en caso
 * de no encontrar el dato.*/
void *hash_borrar(hash_t *hash, const char *clave)
{
	if ( (hash->tamanio / 4) >= hash->cant_listas && hash->tamanio > LONG_INI )
		rehash(hash, hash->tamanio / 2);
	
	size_t pos = hashing(clave, hash->tamanio);
	if(hash->vector[pos] == VACIO) return NULL;
	
	paquete_t* paquete = buscar_paquete(hash, clave, BORRAR, pos);
	if (paquete)
		return desempaquetar(paquete);
	
	return NULL;
}

/* Devuelve el dato guardado en la tabla de hash asociado a la clave o 
 * devuelve NULL en caso de no encontrarse la clave.
 * Pre: el hash fué creado.
 * Post: se devolvió el elemento del hash correspondiente a la clave, o 
 * NULL en caso de no encontrarlo.*/
void *hash_obtener(hash_t *hash, const char *clave)
{
	size_t pos = hashing(clave ,hash->tamanio);
	paquete_t* paquete = buscar_paquete(hash, clave, 0, pos);
	if (paquete)
		return paquete->dato;
	return NULL;
}

/* Devuelve la cantidad de espacios ocupados en la tabla de hash.
 * Pre: la tabla de hash fué creada.
 * Post: se devolvió la cantidad de espacios ocupados en la tabla de hash.*/
size_t hash_cantidad(hash_t *hash)
{
	return hash->cantidad;
}

/* *******************************************************************
 *    IMPLEMENTACION DE FUNCIONES DEL ITERADOR DE LA TABLA DE HASH
 * *******************************************************************/
 
/* Crea un iterador en la tabla de hash ingresada y lo devuelve. Si
 * ocurre un error devuelve NULL.
 * Pre: La tabla de hash fué creada.
 * Post: se devolvió un iterador.*/
hash_iter_t *hash_iter_crear(const hash_t *hash)
{
	hash_iter_t* iter = malloc(sizeof(hash_iter_t));
	if(!iter) return NULL;

	iter->hash = hash;
	iter->pos = 0;
	if(hash->cantidad != 0){
		while(iter->pos < hash->tamanio && hash->vector[iter->pos] == VACIO)
			iter->pos++;
		iter->iter_lista = lista_iter_crear(hash->vector[iter->pos]);
		paquete_t* paquete = lista_iter_ver_actual(iter->iter_lista);
		iter->actual = paquete->clave;
	}else{
		iter->iter_lista = NULL;
		iter->actual = NULL;
	}
	return iter;
}

/* Avanza el iterador a la siguiente posición de la tabla de hash.
 * Devuelve verdadero si el iter avanzó, o falso si está al final del
 * hash.
 * Pre: el iter fué creado.
 * Post: se devolvió verdadero o falso y el iter avanzo una posicion.*/
bool hash_iter_avanzar(hash_iter_t *iter)
{
	if (hash_iter_al_final(iter)) return false; // el iter ya está al final del hash
	
	const hash_t* hash = iter->hash;
	lista_iter_avanzar(iter->iter_lista);
			
	if (lista_iter_al_final(iter->iter_lista)){ // el iter está al final de una lista
		lista_iter_destruir(iter->iter_lista);
		do{
			iter->pos++;		
		}while ( iter->pos < hash->tamanio && hash->vector[iter->pos] == VACIO);
		if(iter->pos == hash->tamanio){ // el iter esta al final de la ultima lista, entonces pasa a estar al final del hash
			iter->iter_lista = NULL;
			iter->pos = iter->hash->tamanio;
			iter->actual = NULL;
			return false;
		}
		iter->iter_lista = lista_iter_crear(hash->vector[iter->pos]);
	}
	paquete_t* paquete = lista_iter_ver_actual(iter->iter_lista);
	iter->actual = paquete->clave;
	return true;
}

/* Devuelve la clave actual en donde esta posicionado el iterador
 * Pre: El iter fué creado.
 * Post: Se devolvió la clave del elemento en el que se encuentra el iter.*/
const char *hash_iter_ver_actual(const hash_iter_t *iter)
{
	return iter->actual;
}

/* Devuelve verdadero si el iter se encuentra al final de la tabla de 
 * hash, devuelve falso en caso contrario
 * Pre: El iterador fué creado.
 * Psot: Se devolvió verdadero o falso.*/
bool hash_iter_al_final(const hash_iter_t *iter)
{
	return (!iter->actual);
}

/* Destruye el iter.
 * Pre: El iter fué creado.
 * Post: Se destruyó el iter.*/
void hash_iter_destruir(hash_iter_t* iter)
{
	lista_iter_destruir(iter->iter_lista);
	free(iter);
}
