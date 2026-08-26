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
    int interacoes =0;
    double x =(x2-x1)/largura; 
    double y = (y2-y1)/altura;

    for(int i1=0;i1<largura;i1++){
        for(double i2=0;i2<altura;i2++){
            
            double c_real = x1+i1*(x);
            double c_imag = y1+i2*(y);
            
            while(((x*x)+(y*y))<=4.0 && interacoes<total){
                int valor = (x*x) + 2*x*y - (y*y);
                printf("valor: %lf  e %ld\n",valor,i);
                interacoes++;
            }
        }
    }


    return 0;
}