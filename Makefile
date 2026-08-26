ConjuntoMandelbrot: main.o
					gcc main.o -o ConjuntoMandelbrot -lm

main.o: main.c
	gcc -c main.c -o main.o -lm


run : ConjuntoMandelbrot
			./ConjuntoMandelbrot -lm

clean : 
		rm -f *.o a.out ConjuntoMandelbrot


.PHONY: run clean