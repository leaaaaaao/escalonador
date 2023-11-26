FLAGS = -ansi -pedantic -Wall

escalonador: obj/main.o obj/filas.o obj/procs.o
	@echo "Compilando..."
	gcc -o escalonador obj/main.o obj/filas.o obj/procs.o $(FLAGS)
	@echo "Feito"	

obj/main.o: main.c
	@mkdir -p obj
	@gcc -c main.c -o obj/main.o

obj/filas.o: filas.c filas.h
	@mkdir -p obj
	@gcc -c filas.c -o obj/filas.o

obj/procs.o: procs.c procs.h
	@mkdir -p obj
	@gcc -c procs.c -o obj/procs.o

clean:
	rm -f obj/*.o escalonador