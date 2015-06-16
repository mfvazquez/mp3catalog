#Compilador
CC = gcc
#Opciones usadas para compilar
CFLAGS = -Wall -pedantic -std=c99 -g
#Nombre del programa principal
CEXE = mp3catalog


all: $(CEXE) prueba_avl prueba_hash prueba_heap prueba_mp3catalog

.SILENT:
menu.o: menu.h menu.c
	$(CC) $(CFLAGS) -c menu.c

.SILENT:
lista.o: lista.h lista.c
	$(CC) $(CFLAGS) -c lista.c

.SILENT:
pila.o: pila.h pila.c
	$(CC) $(CFLAGS) -c pila.c

.SILENT:
hash.o: hash.h hash.c
	$(CC) $(CFLAGS) -c hash.c
	
.SILENT:
heap.o: heap.h heap.c
	$(CC) $(CFLAGS) -c heap.c

.SILENT:
avl.o: avl.h avl.c
	$(CC) $(CFLAGS) -c avl.c

.SILENT:
funciones_auxiliares.o: funciones_auxiliares.h funciones_auxiliares.c
	$(CC) $(CFLAGS) -c funciones_auxiliares.c
	
.SILENT:
lector_canciones.o: lector_canciones.h lector_canciones.c
	$(CC) $(CFLAGS) -c lector_canciones.c

.SILENT:
prueba_hash: lista.o hash.o prueba_hash.c
	$(CC) $(CFLAGS) lista.o hash.o prueba_hash.c -o prueba_hash
	
.SILENT:
prueba_heap: heap.o prueba_heap.c
	$(CC) $(CFLAGS) heap.o prueba_heap.c -o prueba_heap
	
.SILENT:
prueba_avl: pila.o avl.o prueba_avl.c
	$(CC) $(CFLAGS) pila.o avl.o prueba_avl.c -o prueba_avl

.SILENT:
prueba_mp3catalog: lista.o hash.o heap.o avl.o pila.o menu.o funciones_auxiliares.o lector_canciones.o interactivo.o linea_de_comando.o prueba_mp3catalog.c
	$(CC) $(CFLAGS) lista.o hash.o avl.o pila.o heap.o menu.o funciones_auxiliares.o lector_canciones.o interactivo.o linea_de_comando.o prueba_mp3catalog.c -o prueba_mp3catalog
	
.SILENT:
linea_de_comando.o: linea_de_comando.h linea_de_comando.c
	$(CC) $(CFLAGS) -c linea_de_comando.c

.SILENT:
interactivo.o: interactivo.h interactivo.c
	$(CC) $(CFLAGS) -c interactivo.c

.SILENT:
$(CEXE): lista.o hash.o heap.o avl.o pila.o menu.o funciones_auxiliares.o lector_canciones.o interactivo.o linea_de_comando.o $(CEXE).c
	$(CC) $(CFLAGS) lista.o hash.o avl.o menu.o pila.o heap.o funciones_auxiliares.o lector_canciones.o interactivo.o linea_de_comando.o $(CEXE).c -o $(CEXE)

.SILENT:
clean:
	-rm *.o

.SILENT:
cleanAll: clean
	-rm $(CEXE)
	-rm prueba_hash
	-rm prueba_heap
	-rm prueba_avl
	-rm prueba_mp3catalog
	

run: all
	./$(CEXE)	

test: $(CEXE)
	valgrind --leak-check=full ./$(CEXE)

test_hash:
	valgrind --leak-check=full -v ./prueba_hash

test_avl:
	valgrind --leak-check=full -v ./prueba_avl

test_heap:
	valgrind --leak-check=full -v ./prueba_heap
	
test_mp3catalog:
	valgrind --leak-check=full -v ./prueba_mp3catalog