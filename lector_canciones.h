/*
 * lector_canciones.h
 * Herramienta para procesar archivos que contienen datos sobre canciones
 * Copyright: (2011) Margarita Manterola, Maximiliano Curia
 * Licencia: CC-BY-SA 2.5 (ar) ó CC-BY-SA 3.0
 */
#ifndef LECTOR_CANCIONES_H
#define LECTOR_CANCIONES_H

#include <stdbool.h>

/* Tipos de lector, según el tipo de archivo */
#define LECTOR_COLECCION 1
#define LECTOR_PUNTAJES 2

/*
 * Estructura para almacenar la información de las canciones.
 * No es un tipo abstracto de datos, sólo una forma de almacenamiento.
 */
#define TRACK_SIZE   30
#define ARTIST_SIZE  30
#define ALBUM_SIZE   30
#define YEAR_SIZE     4
#define COMMENT_SIZE 28

typedef struct cancion {

    // Nombre del archivo en el disco rígido
    char *archivo;

    // Informacion leida de los archivos mediante idv3
    char titulo[TRACK_SIZE+1];
    char artista[ARTIST_SIZE+1];
    char album[ALBUM_SIZE+1];
    char anyo[YEAR_SIZE+1];
    char comentario[COMMENT_SIZE+1];
    unsigned char track;
    unsigned char genero;

    // Informacion adicional
    // Leída mediante el lector de puntajes
    unsigned int puntos;
    unsigned int votos;
    unsigned int reproducciones;

} cancion_t;

/* Estructura para almacenar los puntajes.  Debe ser unida a la otra luego
 * de ser leída. */
typedef struct puntajes {

    // Nombre del archivo en el disco rígido
    char *archivo;

    // Puntuación correspondiente
    unsigned int puntos;
    unsigned int votos;
    unsigned int reproducciones;

} puntaje_cancion_t;

/* Tipo de datos opaco, para ser utilizado desde el 
   programa principal. */
typedef struct lector_canciones lector_canciones_t;

/*
 * Crea un lector de canciones a partir del nombre de archivo recibido.
 * El tipo de lector indica si es un lector de colecciones o de puntajes.
 *
 * En caso de error, devuelve NULL para el lector de canciones.  El valor
 * de error podrá ser leído mediante la biblioteca errno (man 3 errno).
 *
 * Pre: archivo es una cadena válida.
 * Post: se creó un lector de canciones a partir del archivo, que
 * quedó posicionado en la primera canción.
 */
lector_canciones_t* lector_canciones_crear(const char* archivo, int tipo);

/*
 * Devuelve un puntero a la canción o puntaje actual, o NULL de no haber
 * más datos disponibles en el archivo.
 *
 * ATENCION: La memoria pedida para la canción debe ser liberada 
 * por el usuario.
 *
 * Pre: el lector fue creado exitosamente.
 * Post: se devolvió la canción actual dentro del archivo, o NULL en caso
 * de no haber más canciones.
 */
void* lector_canciones_ver_actual(lector_canciones_t* lector);

/*
 * Avanza a la siguiente canción dentro del archivo. Devuelve true si pudo
 * avanzar, false si ya llegó al final.
 * Pre: el lector fue creado exitosamente.
 * Post: el lector se encuentra en la siguiente canción.
 */
bool lector_canciones_avanzar(lector_canciones_t* lector);

/*
 * Verifica si el lector se encuentra al final del archivo. 
 * Pre: el lector fue creado exitosamente.
 */
bool lector_canciones_al_final(lector_canciones_t* lector);

/*
 * Libera los recursos pedidos por el lector de canciones.
 * Pero NO libera las canciones.
 * Pre: el lector fue creado exitosamente.
 * Post: los recursos del lector fueron liberados
 */
void lector_canciones_destruir(lector_canciones_t* lector);

/*
 * Libera la cancion.
 * Pre: la canción había sido leída con el lector.
 * Post: los recursos de la canción fueron liberados.
 */
void cancion_destruir(cancion_t* cancion);

/*
 * Libera los datos de puntaje
 * Pre: el puntaje había sido leído con el lector.
 * Post: los recursos del puntaje fueron liberados.
 */
void puntaje_destruir(puntaje_cancion_t* puntaje);

#endif
