#include <stdlib.h>
#include "lista.h"

/* ******************************************************************
 *    			   Profesor: Maximiliano Curia
 * -Integrantes del grupo:                  -Padron:
 * 		Vázquez Matías..........................91523
 *		San Pedro Martín........................91461
 * *****************************************************************/


/* ******************************************************************
 *                DEFINICION DE LOS TIPOS DE DATOS
 * *****************************************************************/

typedef struct nodo{
	void* dato;
	struct nodo *prox;
}nodo_t;

struct lista{
	nodo_t *primero;
	nodo_t *ultimo;
	size_t cantidad;
};

struct lista_iter{
	nodo_t* actual;
	nodo_t* anterior;
};

/* ******************************************************************
 *                        IMPLEMENTACION
 * *****************************************************************/
	
// Crea un nodo.
// Post: devuelve un nuevo nodo con un valor.
nodo_t* crear_nodo(void* valor)
{
	/* Defino el nodo. */
	nodo_t* nodo = malloc(sizeof(nodo_t));
	if (!nodo)
		return NULL;
	
	/* Asigno valores a sus respectivos miembros. */
	nodo->prox = NULL;
	nodo->dato = valor;
	
	return nodo;
}
	
// Crea una lista.
// Post: devuelve una nueva lista vacía.
lista_t *lista_crear()
{
	lista_t *lista_nueva = malloc(sizeof(lista_t));
	if (!lista_nueva)
		return NULL;

	lista_nueva->primero = NULL;
	lista_nueva->ultimo = NULL;
	lista_nueva->cantidad = 0;		
		
	return lista_nueva;
	
}
	
// Devuelve verdadero o falso, según si la lista tiene o no elementos agregados.
// Pre: la lista fue creada.
bool lista_esta_vacia(const lista_t *lista)
{
	return (lista->cantidad==0);
}

// Inserta un elemento en una lista vacía.
// Pre: la lista fue creada y está vacía.
// Post: se insertó un elemento a la lista vacía.
void insertar_lista_vacia(lista_t *lista, nodo_t *nodo)
{
	lista->primero = nodo;
	lista->ultimo = nodo;
}
	

// Agrega un nuevo elemento al principio de la lista. Devuelve falso 
// men caso de error.
// Pre: la lista fue creada.
// Post: se agregó un nuevo elemento a la lista, dato se encuentra al inicio
// de la lista.
bool lista_insertar_primero(lista_t *lista, void *dato)
{
	nodo_t* nodo_nuevo = crear_nodo(dato);
	if (!nodo_nuevo)
		return false;
	
	/*Si la lista esta vacía el nodo será el primero y último de la lista. */
	if (lista_esta_vacia(lista))
		insertar_lista_vacia(lista, nodo_nuevo);
	else{
		/* Asignamos a la primer posicion al nuevo nodo */
		nodo_nuevo->prox = lista->primero;
		lista->primero = nodo_nuevo;
	}
	lista->cantidad++;
	return true;
}

// Agrega un nuevo elemento al final de la lista. Devuelve falso 
// en caso de error.
// Pre: la lista fue creada.
// Post: se agregó un nuevo elemento a la lista, dato se encuentra al final
// de la lista.
bool lista_insertar_ultimo(lista_t *lista, void *dato)
{
	/* Creamos el nuevo nodo. */
	nodo_t* nodo_nuevo = crear_nodo(dato);
	if (!nodo_nuevo)
		return false;
	
	/*Si la lista esta vacía el nodo será el primero y último de la lista. */
	if (lista_esta_vacia(lista))
		insertar_lista_vacia(lista, nodo_nuevo);
	else{
		/* Obtenemos el ultimo nodo y le agregamos el nuevo nodo al miembro prox. */
		lista->ultimo->prox = nodo_nuevo;
		lista->ultimo = nodo_nuevo;
	}
	lista->cantidad++;
	return true;
}

// Saca el primer elemento de la lista. Si la lista tiene elementos,
// se quita el primero de la lista, y se devuelve su valor, si está 
// vacía, devuelve NULL.
// Pre: la lista fue creada.
// Post: se devolvió el valor del primer elemento anterior, la lista
// contiene un elemento menos, si la lista no estaba vacía.
void *lista_borrar_primero(lista_t *lista)
{
	/* Verificamos si está vacía */
	if (lista_esta_vacia(lista))
		return NULL;
		
	/* Realizamos las modificaciones a la lista */
	nodo_t *nodo_aux = lista->primero;
	void *dato_aux = lista_ver_primero(lista);
	lista->primero = nodo_aux->prox;
	free(nodo_aux);
	lista->cantidad--;
	return dato_aux;
}
	
// Obtiene el valor del primer elemento de la lista. Si la lista tiene
// elementos, se devuelve el valor del primero, si está vacía devuelve NULL.
// Pre: la lista fue creada.
// Post: se devolvió el primer elemento de la lista, cuando no está vacía.
void *lista_ver_primero(const lista_t *lista)
{
	if (lista_esta_vacia(lista)) 
		return NULL;
	return lista->primero->dato;
}

// Devuelve el valor del largo de la lista.
// Pre: la lista fue creada.
size_t lista_largo(const lista_t *lista)
{
	return lista->cantidad;
}

// Destruye la lista. Si se recibe la función destruir_dato por parámetro,
// para cada uno de los elementos de la lista llama a destruir_dato.
// Pre: la lista fue creada. destruir_dato es una función capaz de destruir
// los datos de la lista, o NULL en caso de que no se la utilice.
// Post: se eliminaron todos los elementos de la lista.
void lista_destruir(lista_t *lista, void destruir_dato(void *))
{
	/* Declaro nodo_aux para recorrer la cola y nodo_para_destruir para 
	 * guardar el nodo que será destruido. */
	nodo_t *nodo_aux = lista->primero, *nodo_para_destruir;

	while(nodo_aux){
		
		/* Destruimos el dato del nodo si se recibe destruir_dato */
		if (destruir_dato && !lista_esta_vacia(lista))
			destruir_dato(nodo_aux->dato);
			
		nodo_para_destruir = nodo_aux;
		nodo_aux = nodo_aux->prox;
		free(nodo_para_destruir);
	}
	free(lista);
}

// Crea un iterador y lo ubica en el primer elemento de la lista.
// Pre: la lista fue creada.
// Post: devuelve un iterador en el primer elemento de la lista.
lista_iter_t *lista_iter_crear(const lista_t *lista)
{
	/* Defino el iterador */
	lista_iter_t* iter = malloc(sizeof(lista_iter_t));
	if (!iter) 
		return NULL;
	
	/* Posicionamos al iterador al comienzo de la lista */
	iter->anterior = NULL;
	iter->actual = lista->primero;
	
	return iter;
}

// Mueve el iterador al siguiente elemento de la lista. En caso de 
// encontrarse al final de la lista devuelve false, caso contrario
// devuelve true.
// Pre: el iterador fue creado.
// Post: el iterador avanzó al siguiente elemento.
bool lista_iter_avanzar(lista_iter_t *iter)
{
	/* Verificamos si esta al final de la lista o si esta vacía */
	if (!iter->actual) 
		return false;
	
	/* Avanzamos al siguiente elemento */
	iter->anterior = iter->actual;
	iter->actual = iter->actual->prox;
	
	return true;
}

// Obtiene el valor del elemento en donde se posiciona el iterador, 
// devuelve NULL en caso de estar al final de la lista o si la lista
// está vacía.
// Pre: el iterador fue creado.
// Post: se devolvió el valor del elemento donde está el iterador.
void *lista_iter_ver_actual(const lista_iter_t *iter)
{
	if (!iter->actual)
		return NULL;
	return iter->actual->dato;
	
}

// Devuelve verdadero o falso, segun si el iterador se encuentra al
// final de la lista.
// Pre: el iterador fue creado.
bool lista_iter_al_final(const lista_iter_t *iter)
{
	return (!iter->actual);
}

// Destruye el iterador.
// Pre: el iterador fue creado.
// Post: se destruyó el iterador.
void lista_iter_destruir(lista_iter_t *iter)
{
	free(iter);
}

// Inserta un nuevo elemento a la lista en la posicion del iterador, 
// devuelve falso en caso de error.
// Pre: la lista y el iterador fueron creados.
// Post: se insertó un nuevo elemento en la lista.
bool lista_insertar(lista_t *lista, lista_iter_t *iter, void *dato)
{
	/* Creamos una variable para verificar que, en caso de ser usadas,
	 * insertar_primero o insertar_ultimo funcionaron correctamente */
	bool exito = true;
	
	/* Si el iterador está al principio de la lista o la lista
	 * está vacía, insertamos en el primero */
	if (!iter->anterior && (exito = lista_insertar_primero(lista, dato))){
		iter->actual = lista->primero;
		return true;
	}
	
	/* Si el iterador está al final de la lista insertamos en el ultimo */
	if (lista_iter_al_final(iter) && (exito = lista_insertar_ultimo(lista, dato))){
		iter->actual = lista->ultimo;
		return true;
	}
	
	/* El iterador no está al final ni al principio de la lista */
	if (exito){
		/* Creamos el nuevo nodo con el dato a insertar */
		nodo_t* nodo_nuevo = crear_nodo(dato);
		if (!nodo_nuevo)
			return false;
		
		/* Ubicamos el nuevo nodo en la posicion en donde se 
		 * encontraba el iterador, iter->actual pasa a ser el nuevo
		 * nodo insertado e iter->anterior se mantiene constante */
		nodo_nuevo->prox = iter->actual;
		iter->actual = nodo_nuevo;
		iter->anterior->prox = nodo_nuevo;
		lista->cantidad++;
		return true;
	}
	
	/* lista_insertar_primero o lista_insertar_ultimo devolvió false */
	return false;
}

// Borra de la lista el elemento en donde se encuentra el iterador y 
// devuelve su valor. Devuelve NULL en caso de estar al final de la lista
// o que la lista esté vacía.
// Pre: la lista y el iterador fueron creados
// Post: Se elminó un elemento de la lista y se devolvió su valor. 
void *lista_borrar(lista_t *lista, lista_iter_t *iter)
{
	/* Retornamos NULL si esta al final de la lista */
	if (lista_iter_al_final(iter) || lista_esta_vacia(lista))
		return NULL;
	
	/* Retornamos lista_borrar_primero si el iterador está al comienzo
	 * de la lista */
	if (!iter->anterior){
		iter->actual = lista->primero->prox;
		return lista_borrar_primero(lista);
	}
	
	/* Creamos variables auxiliares para guardar su dato y liberarla */
	nodo_t* nodo_aux = iter->actual;
	void* dato_aux = nodo_aux->dato;
	
	/* Ubicamos el iterador en su nueva posicion actual */
	iter->actual = iter->actual->prox;
	iter->anterior->prox = iter->actual;
	if (!iter->actual)
		lista->ultimo = iter->anterior;
	lista->cantidad--;
	free(nodo_aux);
	return dato_aux;
}
