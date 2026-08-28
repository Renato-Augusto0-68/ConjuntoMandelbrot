#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>

//Teste de mandelbrot na raça

typedef struct dados{
    double x_p; 
    int larg;
    double total;
    double y_p;
    int i1;
    int inicio;
    int fim;
    int *storage;
}dados;



void *calcularXY_pthrd(void *arg){
    dados *info = (dados *)arg;
    for(int i2=(info->inicio);i2<info->fim;i2++){
        double x=0.0;
        double y=0.0;
        double c_real = -2.0+i2*(info->x_p);
        double c_imag = -1.5+(info->i1)*(info->y_p);
        double interacoes =0.0;
                
        while(((x*x)+(y*y))<=4.0 && interacoes<(info->total)){
                double x_temp = (x*x) - (y*y) + c_real; 
                y= 2*x*y +(c_imag);
                x= x_temp;
                interacoes++;
        }
        double valor = ((255.0*interacoes)/info->total);
        info->storage[(info->i1)*(info->larg) + i2] = (int)valor;            
    }  
}

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
        for(int cont=0;cont<altura;cont++){
            for(int cont2=0;cont2<larg;cont2++){
                fprintf(arq,"%d ",valores[cont*altura+cont2]);
            }
            fprintf(arq,"\n");
        }
    }
    fclose(arq);
}   

int main(/*int argc, char *argv[]*/){
    
    //if (argc==5)
    
    int i=0;
    double total=40.0;
    double x1 = -2.0, x2=1.0;
    double y1 = -1.5,y2=1.5; 
    int num_threads=4;
    int largura =10;
    int altura =6;  
    int *valores1 = (int *) malloc(sizeof(int)*largura*altura);
    int *valores2 = (int *) malloc(sizeof(int)*largura*altura);
    int *valores3 = (int *) malloc(sizeof(int)*largura*altura);
    int *valores4 = (int *) malloc(sizeof(int)*largura*altura);
    for(int modo=1;i<=4;modo++){
        double Z=0.0;
        double x_p =(x2-x1)/largura; 
        double y_p = (y2-y1)/altura;
        for(int i1=0;i1<altura;i1++){          
            if(modo==4){
                dados infos;
                int v_cont = total/num_threads;
                infos.x_p=x_p; 
                infos.total=total;
                infos.larg = largura;
                infos.y_p=y_p;
                infos.i1=i1;
                infos.storage=valores4;
                pthread_t val[num_threads];
                for(int i=0;i<num_threads;i++){
                    int valr_i = v_cont*i;
                    int fim = valr_i + v_cont -1;
                    infos.inicio=valr_i;
                    infos.fim=fim;
                    pthread_t v = val[i];
                    pthread_create(&v,NULL,calcularXY_pthrd,(void*)&infos);
                    pthread_join(v,NULL);
                    
                }  
                
            }    
            if(modo==1){
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
                        valores1[i1*altura + i2] = (int)valor;            
                    }
            }if(modo==2){
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
                        valores2[i1*altura + i2] = (int)valor;            
                    }  
            }if(modo==3){
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
                        valores3[i1*altura + i2] = (int)valor;            
                    }     
            }
        }
        if (modo==1)
            abrirarquivo(modo,valores1,largura,altura);
        if (modo==2)
            abrirarquivo(modo,valores2,largura,altura);
        if (modo==3)
            abrirarquivo(modo,valores3,largura,altura);
        if (modo==4)
            abrirarquivo(modo,valores4,largura,altura);
    }
    free(valores1);
    free(valores4);
    free(valores2);
    free(valores3);
    return 0;
}