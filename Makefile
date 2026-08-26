ConjuntoMandelbrot: main.o
					gcc main.o -o ConjuntoMandelbrot

main.o: main.c
	gcc -c main.c -o main.o


run : ConjuntoMandelbrot
			./ConjuntoMandelbrot

clean : 
		rm -f *.o a.out ConjuntoMandelbrot


.PHONY: run clean