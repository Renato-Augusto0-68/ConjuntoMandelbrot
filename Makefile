ConjuntoMandelbrot: main.o
					gcc main.o -o ConjuntoMandelbrot -fopenmp -lm

main.o: main.c
	gcc -lpthread -c main.c -o main.o -fopenmp -lpthread -lm


run : ConjuntoMandelbrot
			./ConjuntoMandelbrot fopenmp -lpthread -lm

clean : 
		rm -f *.o a.out *.txt *.pgm ConjuntoMandelbrot


.PHONY: run clean