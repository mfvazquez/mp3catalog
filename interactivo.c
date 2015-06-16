/*
 * interactivo.c
 *
 *  Created on: 11/11/2011
 *      Author: Matías Vázquez
 *      Padron: 91523
 *      Professor: Maximiliano Curia
 */

#include "interactivo.h"
#include "linea_de_comando.h"
#include "funciones_auxiliares.h"
#include "menu.h"
#include <stdio.h>
#include <string.h>

struct filtros{
	avl_t*	artistas;
	avl_t* albumes;
	hash_t* generos;
	hash_t* anyos;
};

/*
 * Crea la estructura filtros, que contiene 2 arboles avl y 2 hash.
 * El avl artistas para filtrar las canciones por artista.
 * El avl albumes para filtrar las canciones por album.
 * EL hash generos para filtrar las canciones por genero.
 * El hash anyos para filtrar las canciones por el año.
 */
filtros_t* crear_filtros()
{
	filtros_t* datos = malloc(sizeof(filtros_t));
	if(!datos) return NULL;

	datos->artistas = avl_crear(strcmp,(avl_destruir_dato_t) lista_destruir_canciones);
	if(!datos->artistas){
		free(datos);
		return NULL;
	}
	datos->albumes = avl_crear(strcmp, (avl_destruir_dato_t) lista_destruir_vacia);
	if(!datos->albumes){
		free(datos->artistas);
		free(datos);
		return NULL;
	}
	datos->generos = hash_crear((hash_destruir_dato_t) lista_destruir_vacia);
	if(!datos->generos){
		free(datos->artistas);
		free(datos->albumes);
		free(datos);
		return NULL;
	}
	datos->anyos = hash_crear((hash_destruir_dato_t) lista_destruir_vacia);
	if(!datos->anyos){
		free(datos->artistas);
		free(datos->albumes);
		free(datos->generos);
		free(datos);
		return NULL;
	}
	return datos;
}

/*
 * Destruye la estructura filtros.
 * Pre: La estructura fue creada.
 * Post: se libero el espacio en memoria ocupada por la estructura.
 */
void destruir_filtros(filtros_t* datos)
{
	avl_destruir(datos->albumes);
	hash_destruir(datos->generos);
	hash_destruir(datos->anyos);
	avl_destruir(datos->artistas);
	free(datos);
}

/*
 * Funcion principal del modulo interactivo que da inicio
 * al programa en modo interactivo.
 */
void interactivo(void)
{
	printf("Bienvenido a mp3Catalog.\n");

	filtros_t* datos = crear_filtros();
	if(!datos) return;

	if (crear_datos(datos))
		seleccionar_canciones(datos);

	destruir_filtros(datos);
}

/*
 * Funcion que recopila la informacion de los directorios con la coleccion
 * de las canciones y los puntajes, primero crea un hash con todas las canciones
 * y guarda los puntajes en cada cancion, luego pasa todas las canciones
 * a la estructura filtros.
 * El avl
 */
bool crear_datos(filtros_t* datos)
{
	char* coleccion = malloc(14 * sizeof(char));
	char* puntajes = malloc(14 * sizeof(char));
	strcpy(coleccion, "coleccion.txt");
	strcpy(puntajes, "puntajes.txt");

	if (pedir_archivos(&coleccion, &puntajes)){
		hash_t* canciones =	filtrar(coleccion, NULL, SIN_FILTRO);
		if (canciones){
			agregar_puntajes(canciones, puntajes);
			guardar_canciones(datos, canciones);
			hash_destruir(canciones);
			free(coleccion);
			free(puntajes);
			return true;
		}
	}
	free(coleccion);
	free(puntajes);
	return false;
}

/*
 * Funcion que interactua con el usuario para recopilar la informacion
 * inicial para iniciar el programa. Se puede ingresar la direccion
 * del archivo coleccion y puntajes, en caso de omision se carga la informacion
 * de los archivos coleccion.txt y puntajes.txt del directorio del programa.
 */
bool pedir_archivos(char** coleccion, char** puntajes)
{
	bool terminar_ciclo = false;
	size_t tam = 2;
	char* linea = NULL;
	char* aux_coleccion = NULL;
	char* aux_puntajes = NULL;
	while (!terminar_ciclo){
		menu_archivos();
		if (linea) free(linea);
		linea = leer_texto(tam);
		if (!linea) linea[0] = '4';
		if (linea[0] <= '4' && linea[0] >= '1' && linea[1] == '\0'){
			switch (linea[0]){
				case '1':
					printf("Coleccion:\n");
					aux_coleccion = leer_texto(14);
					if (!aux_coleccion) linea[0] = '4';
					else{
						free(*coleccion); //estaríamos liberando el anterior aux_coleccion
						*coleccion = aux_coleccion;
						break;
					}
				case '2':
					printf("Puntajes:\n");
					aux_puntajes = leer_texto(14);
					if (!aux_puntajes) linea[0] = '4';
					else{
						free(*puntajes); // estaríamos liberando el anterior aux_puntajes
						*puntajes = aux_puntajes;
						break;
					}
				case '3':
					terminar_ciclo = true;
					break;
				case '4':
					if (linea) free(linea);
					return false;
			}
		}
	}
	if (linea) free(linea);
	return true;
}

/*
 * Pasa todas las canciones del hash canciones a la estructura datos.
 * Pre: las dos estructuras deben estar creadas.
 * Post: se pasaron todas las canciones del hash a la estructura datos.
 * Atencion: Si destruye las canciones del hash canciones se perdera
 * la referencia de las canciones en la estructura datos.
 */
void guardar_canciones(filtros_t* datos, hash_t* canciones)
{
	lista_t* lista_artista;
	lista_t* lista_album;
	lista_t* lista_genero;
	lista_t* lista_anyo;

	cancion_t* cancion;
	hash_iter_t* iter = hash_iter_crear(canciones);
	while (!hash_iter_al_final(iter)){
		const char* clave = hash_iter_ver_actual(iter);
		cancion = hash_obtener(canciones, clave);

		char* album = minusculas(cancion->album);
		char* artista = minusculas(cancion->artista);
		char* genero = copiar_genero(cancion);

		// obtenemos la lista de canciones de cada filtro
		lista_artista = avl_obtener(datos->artistas, artista);
		lista_album = avl_obtener(datos->albumes, album);
		lista_genero = hash_obtener(datos->generos, genero);
		lista_anyo = hash_obtener(datos->anyos, cancion->anyo);

		// si no existen las listas creamos la lista
		if (!lista_artista){
			lista_artista = lista_crear();
			avl_guardar(datos->artistas, artista, lista_artista);
		}
		if (!lista_album){
			lista_album = lista_crear();
			avl_guardar(datos->albumes, album, lista_album);
		}
		if (!lista_genero){
			lista_genero = lista_crear();
			hash_guardar(datos->generos, genero, lista_genero);
		}
		if (!lista_anyo){
			lista_anyo = lista_crear();
			hash_guardar(datos->anyos, cancion->anyo, lista_anyo);
		}

		// agregamos la cancion a la lista
		lista_insertar_primero(lista_artista, cancion);
		lista_insertar_primero(lista_album, cancion);
		lista_insertar_primero(lista_genero, cancion);
		lista_insertar_primero(lista_anyo, cancion);


		free(artista);
		free(album);
		free(genero);
		hash_iter_avanzar(iter);
	}
	hash_iter_destruir(iter);
}

/*
 * Funcion que interactua con el usuario para seleccionar
 * los filtros, criterios y cantidad de canciones a mostrar.
 */
void seleccionar_canciones(filtros_t* datos)
{
	char* linea = NULL;
	char* filtro = malloc(13 * sizeof(char));
	int tipo_filtro = SIN_FILTRO;
	int tipo_criterio = RANDOM;
	int cantidad = -1;
	lista_t* lista_canciones = NULL;
	heap_t* canciones_elegidas = NULL;

	while (1){
		menu_seleccion();
		if (linea) free(linea);
		linea = leer_texto(2);
		if (!linea) linea[0] = '7';
		if (linea[0] <= '7' && linea[0] >= '1' && linea[1] == '\0'){
			switch (linea[0]){
				case '1':
					tipo_filtro = pedir_filtro(&filtro);
					if(!filtro) linea[0] = '7';
					else break;
				case '2':
					tipo_criterio = pedir_criterio();
					if (tipo_criterio == -1) linea[0] = '7';
					else break;
				case '3':
					printf("Artistas:\n\n");
					avl_in_order(datos->artistas, imprimir_artistas, NULL);
					printf("\n");
					break;
				case '4':
					printf("Albumes:\n\n");
					avl_in_order(datos->albumes, imprimir_albumes, NULL);
					printf("\n");
					break;
				case '5':
					lista_canciones = obtener_canciones(datos, tipo_filtro, filtro);
					canciones_elegidas = aplicar_criterio_interactivo(lista_canciones, tipo_criterio);
					if(tipo_filtro == SIN_FILTRO || lista_esta_vacia(lista_canciones))
						lista_destruir(lista_canciones, NULL);
					lista_canciones = NULL;
					listar_canciones(canciones_elegidas, cantidad, false);
					heap_destruir(canciones_elegidas,free); // free libera los paquetes
					canciones_elegidas = NULL;
					break;
				case '6':
					cantidad = pedir_cantidad();
					if (cantidad == -1) linea[0] = '7';
					else break;
				case '7':
					if (linea) free(linea);
					free(filtro);
					if (canciones_elegidas) heap_destruir(canciones_elegidas,free); // free libera los paquetes
					if (lista_canciones && tipo_filtro == SIN_FILTRO) lista_destruir(lista_canciones,NULL);
					return;
			}
		}
	}
}

/*
 * Guarda las canciones de la lista a un heap en base al criterio seleccionado.
 * ATENCION: el heap devuelto debe ser liberado.
 * Pre: la lista fue creado y criterio es algun criterio valido definido
 * en linea_de_comando.h
 * Post: se devolvio un heap con las canciones guardadas en base al criterio elegido.
 */
heap_t* aplicar_criterio_interactivo(lista_t* lista, int criterio)
{
	heap_t* heap = heap_crear((cmp_func_t)comparar);
	paquete_t* paquete;
	cancion_t* cancion;
	int valor;
	lista_iter_t* iter = lista_iter_crear(lista);
	while(!lista_iter_al_final(iter)){
		cancion = lista_iter_ver_actual(iter);

		if(criterio == PUNTAJE) valor = cancion->puntos;
		else if(criterio == MAS_ESCUCHADAS) valor = cancion->reproducciones;
		else if(criterio == MENOS_ESCUCHADAS) valor = -1 *(cancion->reproducciones);
		else if(criterio == RANDOM) valor = rand();

		paquete = empaquetar(cancion, valor);
		heap_encolar(heap, paquete);
		lista_iter_avanzar(iter);
	}
	lista_iter_destruir(iter);
	return heap;
}

/*
 * Funcion que ejecuta a lista_destruir y destruye a la lista y a las
 * canciones almacenadas.
 * Pre: la lista fue creada.
 * Post: se libero el espacio ocupado por la lista y las canciones almacenadas en ella.
 */
void lista_destruir_canciones(lista_t* lista)
{
	lista_destruir(lista, (avl_destruir_dato_t) cancion_destruir);
}

/*
 * Funcion que ejecuta a lista_destruir y solo destruye la lista.
 * Pre: la lista fue creada.
 * Post: se libero el espacio ocupado por la lista.
 */
void lista_destruir_vacia(lista_t* lista)
{
	lista_destruir(lista, NULL);
}

/*
 * Funcion utilizada para pasarla como parametro en avl_in_order(ver avl.h)
 * para que imprima los artistas de las canciones almacenadas en el avl.
 */
bool imprimir_artistas(const char* clave, void* dato, void* extra)
{
	lista_t* lista = dato;
	cancion_t* cancion = lista_ver_primero(lista);
	printf("%s\n",cancion->artista);
	return true;
}

/*
 * Funcion utilizada para pasarla como parametro en avl_in_order(ver avl.h)
 * para que imprima los albumes de las canciones almacenadas en el avl.
 */
bool imprimir_albumes(const char* clave, void* dato, void* extra)
{
	lista_t* lista = dato;
	cancion_t* cancion = lista_ver_primero(lista);
	printf("%s\n",cancion->album);
	return true;
}

/*
 * Funcion que interactua con el usuario para pedirle el filtro
 * y el tipo de filtro.
 * Pre: Filtro debe ser un espacio recervado de memoria.
 */
int pedir_filtro(char** filtro)
{
	int tipo_filtro;
	bool continuar_ciclo = true;
	char* linea = NULL;
	char* texto[4] = {"Artista:", "Album:" ,"Genero:" ,"Año:" };

	while(continuar_ciclo){
		menu_filtro();
		if (linea) free(linea);
		linea = leer_texto(2);
		if (!linea){
			free(*filtro);
			*filtro = NULL;
			continuar_ciclo = false;
		}

		if (linea[0] >= '1' && linea[0] <= '5' && linea[1] == '\0'){
			tipo_filtro = atoi(&linea[0]);
			if(tipo_filtro != 5){
				printf("%s ", texto[tipo_filtro - 1]);
				free(*filtro);
				*filtro = leer_texto(13);
			}
			continuar_ciclo = false;
		}
		if (linea) free(linea);
	}
	return tipo_filtro;
}

/*
 * Funcion que interactua con el usuario para pedirle el criterio.
 */
int pedir_criterio()
{
	int tipo_criterio;
	bool continuar_ciclo = true;
	char* linea = NULL;
	while(continuar_ciclo){
		menu_criterio();
		if (linea) free(linea);
		linea = leer_texto(2);
		if (!linea) return -1;
		if (linea[0] >= '1' && linea[0] <= '5' && linea[1] == '\0'){
			tipo_criterio = atoi(&linea[0]);
			continuar_ciclo = false;
		}
	}
	if (linea) free(linea);
	return tipo_criterio;
}

/*
 * Funcion que interactua con el usuario para pedirle la cantidad
 * de canciones a mostrar.
 */
int pedir_cantidad()
{
	size_t cantidad;
	char* linea = NULL;
	while (1){
		printf("Cantidad maxima de canciones a mostrar:");
		if (linea) free(linea);
		linea = leer_texto(3);
		if(!linea) return -1;
		int exitos = 0;
		int i;
		for (i = 0; linea[i] != '\0'; i++){
			if (linea[i] >= '0' && linea[i] <= '9')
				exitos++;
		}
		if(exitos == i && linea[0] != '\0') break;
	}
	cantidad = atoi(linea);
	if (linea) free(linea);
	return cantidad;
}

/*
 * Funcion utilizada para pasarla como parametro en avl_in_order(ver avl.h)
 * que copiara todas las canciones de todo el avl a lista2.
 */
bool todas_las_canciones(const char* clave, void* lista1, void* lista2 )
{
	lista_iter_t* iter = lista_iter_crear(lista1);
	while(!lista_iter_al_final(iter)){
		cancion_t* cancion = lista_iter_ver_actual(iter);
		lista_insertar_ultimo(lista2, cancion);
		lista_iter_avanzar(iter);
	}
	lista_iter_destruir(iter);
	return true;
}

/*
 * Funcion que devuelve la lista de canciones del filtro elegido, si se elige
 * el filtro SIN_FILTRO(ver linea_de_comando.h) guardara todas las canciones
 * del avl artistas en una lista, en ese caso el usuario debera liberar la lista
 * creada para almacenar todas las canciones.
 * Pre: datos fue creado, tipo_filtro debe ser alguno de los filtros definidos
 * en linea_de_comando.h
 * Post: se devolvio la lista con las canciones filtradas.
 */
lista_t* obtener_canciones(filtros_t* datos, int tipo_filtro, char* filtro)
{
	lista_t* lista_canciones = NULL;

	if (tipo_filtro == SIN_FILTRO){
		lista_canciones = lista_crear();
		avl_in_order(datos->artistas, todas_las_canciones, lista_canciones);
	}else if(tipo_filtro == ARTISTA){
		char* artista = minusculas(filtro);
		lista_canciones = avl_obtener(datos->artistas, artista);
		free(artista);
	}else if(tipo_filtro == ALBUM){
		char* album = minusculas(filtro);
		lista_canciones = avl_obtener(datos->albumes, album);
		free(album);
	}else if(tipo_filtro == GENERO){
		char* genero = malloc(4 * sizeof(char));
		sprintf(genero, "%3s", filtro);
		lista_canciones = hash_obtener(datos->generos, genero);
		free(genero);
	}else if(tipo_filtro == ANYO)
		lista_canciones = hash_obtener(datos->anyos, filtro);

	if (!lista_canciones) lista_canciones = lista_crear();
	return lista_canciones;
}
