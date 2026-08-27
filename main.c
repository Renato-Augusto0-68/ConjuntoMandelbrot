#include <stdio.h>
#include <stdlib.h>
#include <math.h>
//Teste de mandelbrot na raça

int main(/*int argc, char *argv[]*/){
    int i=0;

    double total=10;
    double Z=0.0;
    double x1 = -2.0, x2=1.0;
    double y1 = -1.5,y2=1.5; 
    int largura =6;
    int altura =6;
    double *valores = (double *) malloc(sizeof(double)*largura*altura);    
    double x =(x2-x1)/largura; 
    double y = (y2-y1)/altura;

    for(int i1=0;i1<largura;i1++){
        
        for(double i2=0;i2<altura;i2++){
            double interacoes =0.0;

            double c_real = x1+i1*(x);
            double c_imag = y1+i2*(y);
            
            while(((x*x)+(y*y))<=4.0 && interacoes<total){
                double x_temp = (x*x) - (y*y) + x1; 
                y=2*x*y +(y1);
                x=x_temp;
                interacoes++;
            }
            if(interacoes==total){
                double valor = (interacoes/total)*255;
                *valores = valor;
                valores++;
            }
        }
    }


    return 0;
}