#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>

//Teste de mandelbrot na raça

void abrirarquivo(int mode, double *valores,int larg){
    FILE *arq;
    if (mode==1)
        arq = fopen("mandelbrot_rass_serial.pgm","w");
    if (mode==2)
        arq=fopen("mandelbrot_rass_openmp.pgm","w");
    if (mode==3)
        arq = fopen("mandelbrot_rass_pthreads1.pgm","w");
    if (mode==4)
        arq=fopen("mandelbrot_rass_pthreads2.pgm","w");
    
    if(arq!=NULL){
        int cont =0;
        while(valores!=NULL){
            fprintf(arq,"%.2lf ",*valores);
            if(cont==larg){
                fprintf(arq,"\n");
                cont = 0;
            }else{
                cont++;}
            valores ++;
        }
    }
    fclose(arq);
}   

int main(/*int argc, char *argv[]*/){
    int i=0;
    double total=50.0;
    double Z=0.0;
    double x1 = -2.0, x2=1.0;
    double y1 = -1.5,y2=1.5; 
    int largura =4;
    int altura =4;
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
                double valor = (interacoes/total)*255.0;
                *valores = valor;
                valores++;
            }
        }

        abrirarquivo(1,valores,largura);
    }
    return 0;
}