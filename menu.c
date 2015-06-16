/*
 * menu.c
 *
 *  Created on: 15/11/2011
 *      Author: Matías Vázquez
 *      Padron: 91523
 *      Professor: Maximiliano Curia
 */

#include <stdio.h>

/*
 * Imprime en pantalla el menu de archivos.
 */
void menu_archivos(void)
{
	printf("1 - Importar coleccion.\n");
	printf("2 - Importar puntajes.\n");
	printf("3 - Siguiente.\n");
	printf("4 - Salir.\n");
}

/*
 * Imprime en pantalla el menu seleccion.
 */
void menu_seleccion(void)
{
	printf("1 - Elegir filtro.\n");
	printf("2 - Elegir criterio.\n");
	printf("3 - Listar artistas.\n");
	printf("4 - Listar albumes.\n");
	printf("5 - Listar canciones.\n");
	printf("6 - Cantidad de canciones.\n");
	printf("7 - Salir.\n");
}

/*
 * Imprime en pantalla el menu de filtros.
 */
void menu_filtro(void)
{
	printf("Seleccione un filtro:\n");
	printf("1 - Artista.\n");
	printf("2 - Album.\n");
	printf("3 - Genero.\n");
	printf("4 - Año.\n");
	printf("5 - Sin filtro.\n");
}

/*
 * Imprime el menu de criterios.
 */
void menu_criterio(void)
{
	printf("Seleccione un criterio:(En caso de no elegir criterio se utiliza el criterio aleatorio.)\n");
	printf("1 - Puntaje.\n");
	printf("2 - Mas escuchadas.\n");
	printf("3 - Menos escuchadas.\n");
	printf("4 - Aleatorio.\n");
}
