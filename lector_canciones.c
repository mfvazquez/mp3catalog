/*
 * lector_canciones.c
 * Herramienta para procesar un archivo que contiene datos sobre canciones
 * Copyright: (2011) Margarita Manterola, Maximiliano Curia
 * Licencia: CC-BY-SA 2.5 (ar) ó CC-BY-SA 3.0
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "lector_canciones.h"

/* Tipo de datos opaco, para ser utilizado desde el 
   programa principal. */
struct lector_canciones {
    FILE* archivo;
    int tipo;
    void* actual;
};

/* **********************************************
 * *********  FUNCIONES AUXILIARES  *************
 * **********************************************/

#define BUFSIZE 128

/* Función auxiliar para leer una línea de un archivo. */
char* lector_leer_linea(FILE* archivo){
    
    size_t tam = BUFSIZE; 
    char* buffer = malloc(tam * sizeof(char));
    if (!buffer) return NULL;

    // Lee hasta el enter o hasta el fin de archivo
    size_t i = 0;
    int c = fgetc(archivo);
    while ( c != '\n' && c != EOF ) {
    
        buffer[i] = c;  

        if (i+1 >= tam) { 
            tam *= 2;
            char* aux = realloc(buffer, tam * sizeof(char));
            if (!aux) break;
            buffer = aux;
        }

        c = fgetc(archivo);
        i++;
    }

	// Si el string está vacío, devuelve NULL
	if (i == 0) {
		free(buffer);
		return NULL;
	}

	// Terminador de cadenas
    buffer[i] = '\0';

    /* Reduce la memoria pedida al minimo. */
    char* aux = realloc(buffer, (i+1) * sizeof(char));
    if (!aux) return buffer;
    return aux;
}

/* Si había un separador en la línea, devuelve un arreglo de 
 * 2 posiciones con punteros que apuntan a la línea recibida.
 * Si no había separador, devuelve NULL.
 *
 * En el caso de que no sea NULL, la memoria del arreglo la debe
 * liberar el usuario.
 */
char** lector_split(char* linea, char sep) {
    
    /* Busca el separador en la cadena*/
    char *c = strchr(linea, sep);
    if (!c) return NULL;

    /* Reemplaza el : por un 0. */
    size_t pos = c - linea;
    linea[pos] = '\0';

    char **partes = malloc( 2*sizeof(char*));

    /* Parte antes del separador. */
    partes[0] = linea;
    /* Parte despues del separador.  Suma 2 para saltear el espacio. */
    partes[1] = linea+pos+2;

    return partes;
}

/* Guarda cada campo en la estructura cancion, según corresponda. */
void guardar_campo_cancion(void* c, char** partes, char* linea) {

	cancion_t* cancion = (cancion_t*) c;

	if (! partes) {
		cancion->archivo = linea;
		return;
	}

    if (strcmp(partes[0], "title") == 0) {
        strncpy(cancion->titulo, partes[1], TRACK_SIZE+1);
    } else if (strcmp(partes[0], "artist") == 0) {
        strncpy(cancion->artista, partes[1], ARTIST_SIZE+1);
    } else if (strcmp(partes[0], "album") == 0) {
        strncpy(cancion->album, partes[1], ALBUM_SIZE+1);
    } else if (strcmp(partes[0], "year") == 0) {
        strncpy(cancion->anyo, partes[1], YEAR_SIZE+1);
    } else if (strcmp(partes[0], "comment") == 0) {
        strncpy(cancion->comentario, partes[1], COMMENT_SIZE+1);
    } else if (strcmp(partes[0], "trackno") == 0) {
        cancion->track = atoi(partes[1]);
    } else if (strcmp(partes[0], "genre") == 0) {
        cancion->genero = atoi(partes[1]);
    }
}

/* Guarda cada campo en la estructura puntaje_cancion, según corresponda. */
void guardar_campo_puntaje(void* p, char** partes, char* linea) {

	puntaje_cancion_t* puntaje = (puntaje_cancion_t*) p;

	if (! partes) {
		puntaje->archivo = linea;
		return;
	}

    if (strcmp(partes[0], "listens") == 0) {
        puntaje->reproducciones = atoi(partes[1]);
    } else if (strcmp(partes[0], "votes") == 0) {
        puntaje->votos = atoi(partes[1]);
    } else if (strcmp(partes[0], "points") == 0) {
        puntaje->puntos = atoi(partes[1]);
    }
}

/* Procesa el archivo línea por línea hasta terminar la canción */
void* leer_estructura(lector_canciones_t* lector, void* estructura,	void guardar_campos(void*, char**, char*))
{
    char *linea = lector_leer_linea(lector->archivo);

	/* Si no pudo leer ni una línea, la estructura está vacía. */
	if (!linea) {
		free(estructura);
		return NULL;
	}

	/* Lee hasta encontrar una línea vacía*/
    while (linea) {

		// Divide y guarda
        char** partes = lector_split(linea, ':');
        guardar_campos(estructura, partes, linea);

		// Libera lo que hay que liberar
		if (partes) free(linea);
		free(partes);

		// Proxima línea
        linea = lector_leer_linea(lector->archivo);
    }
	return estructura;
}

/*
 * Lee una canción
 */
bool lector_leer_cancion(lector_canciones_t* lector) {

    /* Memoria para la próxima canción. */
    cancion_t *cancion = malloc(sizeof(cancion_t));
    if (!cancion) return false;

    /* Campos que no se leen con este lector, quedan en cero. */
    cancion->puntos = 0;
    cancion->votos = 0;
    cancion->reproducciones = 0;

	/* Lee la cancion completa */
	lector->actual = leer_estructura(lector, cancion, guardar_campo_cancion);

	/* Si no leyó nada, sale con error*/
	if (!lector->actual) return false;
	return true;
}

/*
 * Lee un puntaje
 */
bool lector_leer_puntaje(lector_canciones_t* lector) {

    /* Memoria para la próxima canción. */
    puntaje_cancion_t *puntaje = malloc(sizeof(puntaje_cancion_t));
    if (!puntaje) return false;

    /* Los campos se inicializan en cero. */
    puntaje->puntos = 0;
    puntaje->votos = 0;
    puntaje->reproducciones = 0;

    /* Lee el puntaje completo. */
	lector->actual = leer_estructura(lector, puntaje, guardar_campo_puntaje);

	/* Si no leyó nada, sale con error*/
	if (!lector->actual) return false;
	return true;
}


/* **********************************************
 * *********  PRIMITIVAS DEL LECTOR *************
 * **********************************************/

/*
 * Crea un lector de canciones a partir del nombre de archivo recibido.
 * En caso de error, devuelve NULL para el lector de canciones.
 */
lector_canciones_t* lector_canciones_crear(const char* archivo, int tipo) {

    /* Pide la memoria para el lector */
    lector_canciones_t *lector = malloc(sizeof(lector_canciones_t));
    if (!lector) {
        return NULL;
    }

    /* Abre el archivo */
    lector->archivo = fopen(archivo, "r");

    /* De no abrirse correctamente, quedará seteada la variable errno.  Es
     * responsabilidad del usuario manejar el error. */
    if (! lector->archivo) {
        free(lector);
        return NULL;
    }

    lector->tipo = tipo;

    /* Lee la primera cancion. */
    lector_canciones_avanzar(lector);
    return lector;
}

/*
 * Devuelve un puntero a la canción actual, o NULL de no haber más
 * canciones.
 */
void* lector_canciones_ver_actual(lector_canciones_t* lector) {
    return lector->actual;
}

/*
 * Avanza a la siguiente canción dentro del archivo. Devuelve true si pudo
 * avanzar, false si ya llegó al final.
 */
bool lector_canciones_avanzar(lector_canciones_t* lector) {

    /* Si llegó al fin de archivo, pone la cancion en NULL y sale. */
    if (feof(lector->archivo)) {
        lector->actual = NULL;
        return false;
    }

    /* Avanza en el archivo según el tipo*/
    if (lector->tipo == LECTOR_COLECCION) {
        return lector_leer_cancion(lector);
    } else if (lector->tipo == LECTOR_PUNTAJES) {
        return lector_leer_puntaje(lector);
    }

    /* Si es de otro tipo, devuelve false. */
    return false;
}

/*
 * Verifica si el lector se encuentra al final del archivo. 
 * Pre: el lector fue creado exitosamente.
 */
bool lector_canciones_al_final(lector_canciones_t* lector) {
    return lector->actual == NULL;
}

/*
 * Libera los recursos pedidos por el lector de canciones.
 * Pero NO libera las canciones.
 */
void lector_canciones_destruir(lector_canciones_t* lector) {
    fclose(lector->archivo);
    free(lector);
}

void cancion_destruir(cancion_t *cancion) {
    free(cancion->archivo);
    free(cancion);
}

void puntaje_destruir(puntaje_cancion_t *puntaje) {
    free(puntaje->archivo);
    free(puntaje);
}

