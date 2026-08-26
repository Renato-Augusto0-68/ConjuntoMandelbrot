#include <stdio.h>
#include <stdlib.h>
#include <math.h>
//Teste de mandelbrot na raça

int main(/*int argc, char *argv[]*/){
    int i=0;

    int total=10;
    double Z=0.0;
    double x1 = -2.0, x2=1.0;
    double y1 = -1.5,y2=1.5; 
    int largura =6;
    int altura =6;

    double x =(x2-x1)/largura; 
    double y = (y2-y1)/altura;

    for(long int i=0;i<12;i++){
        for(double i1=0.0;i1<x;i1+=0.001){

            for(double i2=0.0;i2<y;i2+=0.001){
                double valor = x+y - i;
                printf("valor: %lf  e %ld\n",valor,i);
            }
        }
    }


    return 0;
}