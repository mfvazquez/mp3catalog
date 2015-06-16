#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "lector_canciones.h"


int main(void)
{
    printf("main\n");
    lector_canciones_t* lector =  lector_canciones_crear(
		"coleccion.txt", LECTOR_COLECCION);
    if ( ! lector ) {
        fprintf(stderr, "ERROR: %s\n", strerror(errno) );
        return 1;
    }
    printf("Creado\n");

    int contador = 0;
    while ( ! lector_canciones_al_final(lector) ) {
        cancion_t *cancion = lector_canciones_ver_actual(lector);

        printf("Archivo: %s\n", cancion->archivo);
        printf("Titulo: %s\n", cancion->titulo);
        printf("Artista: %s\n", cancion->artista);
        printf("\n");

        contador++;
        cancion_destruir(cancion);

        lector_canciones_avanzar(lector);
    }

    lector_canciones_destruir(lector);
    printf("Total: %d\n", contador);
    return 0;
}

