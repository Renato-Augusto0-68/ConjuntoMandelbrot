#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>

//Teste de mandelbrot na raça

void abrirarquivo(int mode, int *valores,int larg, int altura){
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
        for(int cont=0;cont<larg;cont++){
            for(int cont2=0;cont2<altura;cont2++){
                fprintf(arq,"%d ",valores[cont*larg+cont2]);
            }
            fprintf(arq,"\n");
        }
    }
    fclose(arq);
}   

int main(/*int argc, char *argv[]*/){
    int i=0;
    double total=50.0;
   
    double x1 = -2.0, x2=1.0;
    double y1 = -1.5,y2=1.5; 
    int largura =4;
    int altura =4;
    double Z=0.0;   
    double x_p =(x2-x1)/largura; 
    double y_p = (y2-y1)/altura;
    
    int *valores = (int *) malloc(sizeof(int)*largura*altura);
    for(int i1=0;i1<altura;i1++){
        
        for(int i2=0;i2<largura;i2++){
            double x=0.0;
            double y=0.0;
            double c_real = x1+i2*(x_p);
            double c_imag = y1+i1*(y_p);
            double interacoes =0.0;
            
            while(((x*x)+(y*y))<=4.0 && interacoes<total){
                double x_temp = (x*x) - (y*y) + c_real; 
                y= 2*x*y +(c_imag);
                x= x_temp;
                interacoes++;
            }
            double valor = ((255.0*interacoes)/total);
            valores[i1*largura + i2] = (int)valor;
            
            
        }  
    }abrirarquivo(1,valores,largura,altura);
    return 0;
}