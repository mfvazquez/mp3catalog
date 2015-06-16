/*
 * mp3catalog.c
 *
 *  Created on: 04/11/2011
 *      Author: Matias Vazquez
 *      Padron: 91523
 *      Professor: Maximiliano Curia
 */

#include "linea_de_comando.h"
#include "interactivo.h"

/*
 * Funcion principal del programa, llama al modo interactivo
 * si no se ingresan opciones, caso contrario modo linea de comando
 */
int main(int argc, char** argv)
{
	if (argc > 1)
		linea_de_comando(argc, argv);
	else
		interactivo();
	return 0;
}
