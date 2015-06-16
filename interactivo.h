/*
 * interactivo.h
 *
 *  Created on: 11/11/2011
 *      Author: Matías Vázquez
 *      Padron: 91523
 *      Professor: Maximiliano Curia
 */

#ifndef INTERACTIVO_H_
#define INTERACTIVO_H_

#include "avl.h"
#include "hash.h"
#include "heap.h"
#include "lista.h"
#include "lector_canciones.h"

typedef struct filtros filtros_t;

/*
 * Crea la estructura filtros, que contiene 2 arboles avl y 2 hash.
 * El avl artistas para filtrar las canciones por artista.
 * El avl albumes para filtrar las canciones por album.
 * EL hash generos para filtrar las canciones por genero.
 * El hash anyos para filtrar las canciones por el año.
 */
filtros_t* crear_filtros();

/*
 * Destruye la estructura filtros.
 * Pre: La estructura fue creada.
 * Post: se libero el espacio en memoria ocupada por la estructura.
 */
void destruir_filtros(filtros_t* datos);

/*
 * Funcion principal del modulo interactivo que da inicio
 * al programa en modo interactivo.
 */
void interactivo(void);

/*
 * Funcion que recopila la informacion de los directorios con la coleccion
 * de las canciones y los puntajes, primero crea un hash con todas las canciones
 * y guarda los puntajes en cada cancion, luego pasa todas las canciones
 * a la estructura filtros.
 * El avl
 */
bool crear_datos(filtros_t* datos);

/*
 * Funcion que interactua con el usuario para recopilar la informacion
 * inicial para iniciar el programa. Se puede ingresar la direccion
 * del archivo coleccion y puntajes, en caso de omision se carga la informacion
 * de los archivos coleccion.txt y puntajes.txt del directorio del programa.
 */
bool pedir_archivos(char** coleccion, char** puntajes);

/*
 * Pasa todas las canciones del hash canciones a la estructura datos.
 * Pre: las dos estructuras deben estar creadas.
 * Post: se pasaron todas las canciones del hash a la estructura datos.
 * Atencion: Si destruye las canciones del hash canciones se perdera
 * la referencia de las canciones en la estructura datos.
 */
void guardar_canciones(filtros_t* datos, hash_t* canciones);

/*
 * Funcion que interactua con el usuario para seleccionar
 * los filtros, criterios y cantidad de canciones a mostrar.
 */
void seleccionar_canciones(filtros_t* datos);

/*
 * Guarda las canciones de la lista a un heap en base al criterio seleccionado.
 * ATENCION: el heap devuelto debe ser liberado.
 * Pre: la lista fue creado y criterio es algun criterio valido definido
 * en linea_de_comando.h
 * Post: se devolvio un heap con las canciones guardadas en base al criterio elegido.
 */
heap_t* aplicar_criterio_interactivo(lista_t* lista, int criterio);

/*
 * Funcion que ejecuta a lista_destruir y destruye a la lista y a las
 * canciones almacenadas.
 * Pre: la lista fue creada.
 * Post: se libero el espacio ocupado por la lista y las canciones almacenadas en ella.
 */
void lista_destruir_canciones(lista_t* lista);

/*
 * Funcion que ejecuta a lista_destruir y solo destruye la lista.
 * Pre: la lista fue creada.
 * Post: se libero el espacio ocupado por la lista.
 */
void lista_destruir_vacia(lista_t* lista);
/*
 * Funcion utilizada para pasarla como parametro en avl_in_order(ver avl.h)
 * para que imprima los artistas de las canciones almacenadas en el avl.
 */
bool imprimir_artistas(const char* clave, void* dato, void* extra);

/*
 * Funcion utilizada para pasarla como parametro en avl_in_order(ver avl.h)
 * para que imprima los albumes de las canciones almacenadas en el avl.
 */
bool imprimir_albumes(const char* clave, void* dato, void* extra);

/*
 * Funcion que interactua con el usuario para pedirle el filtro
 * y el tipo de filtro.
 */
int pedir_filtro(char** filtro);

/*
 * Funcion que interactua con el usuario para pedirle el criterio.
 */
int pedir_criterio();

/*
 * Funcion que interactua con el usuario para pedirle la cantidad
 * de canciones a mostrar.
 */
int pedir_cantidad();

/*
 * Funcion utilizada para pasarla como parametro en avl_in_order(ver avl.h)
 * que copiara todas las canciones de todo el avl a lista2.
 */
bool todas_las_canciones(const char* clave, void* lista1, void* lista2 );

/*
 * Funcion que devuelve la lista de canciones del filtro elegido, si se elige
 * el filtro SIN_FILTRO(ver linea_de_comando.h) guardara todas las canciones
 * del avl artistas en una lista, en ese caso el usuario debera liberar la lista
 * creada para almacenar todas las canciones.
 * Pre: datos fue creado, tipo_filtro debe ser alguno de los filtros definidos
 * en linea_de_comando.h
 * Post: se devolvio la lista con las canciones filtradas.
 */
lista_t* obtener_canciones(filtros_t* datos, int tipo_filtro, char* filtro);

#endif /* INTERACTIVO_H_ */
