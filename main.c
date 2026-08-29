#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include <time.h>
#define TIME_WRITE 5
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
    if(mode==5){
        arq=fopen("times.txt","w");
    }

    if (mode!=5){
        if(arq!=NULL){
            for(int cont=0;cont<altura;cont++){
                for(int cont2=0;cont2<larg;cont2++){
                    fprintf(arq,"%d ",valores[cont*larg+cont2]);
                }
                fprintf(arq,"\n");
            }
        }
    }else{
        if(arq!=NULL){
            for(int i=0;i<4;i++){
                if(i==0)
                    fprintf(arq,"Serial: %f\n",valores[i]);
                if (i==1)
                    fprintf(arq,"OpenMP: %f\n",valores[i]);
                if (i ==2)
                    fprintf(arq,"Pthreads1: %f\n",valores[i]);
                if (i==3)
                    fprintf(arq,"Pthreads2: %f\n",valores[i]);
            }
        }
    }
    fclose(arq);

}   

int main(/*int argc, char *argv[]*/){
    
    //if (argc==5)
    
    double total=40.0;
    double x1 = -2.0, x2=1.0;
    double y1 = -1.5,y2=1.5; 
    int num_threads=4;
    int largura =10;
    clock_t  start_time,end_time;
    clock_t tempos[4];
    
    int altura =6;  
    int *valores1 = (int *) malloc(sizeof(int)*largura*altura);
    int *valores2 = (int *) malloc(sizeof(int)*largura*altura);
    int *valores3 = (int *) malloc(sizeof(int)*largura*altura);
    int *valores4 = (int *) malloc(sizeof(int)*largura*altura);
    for(int modo=1;modo<=4;modo++){
        double Z=0.0;
        double x_p =(x2-x1)/largura; 
        double y_p = (y2-y1)/altura;
        dados infos[num_threads];
        dados infos3[num_threads/2];
        int v_cont = largura/num_threads;  
        pthread_t val[num_threads];
        for(int i1=0;i1<altura;i1++){          
            if(modo==4){ 
                start_time=clock();
                for(int i=0;i<num_threads;i++){
                    infos[i].x_p=x_p; 
                    infos[i].total=total;
                    infos[i].larg = largura;
                    infos[i].y_p=y_p;
                    infos[i].i1=i1;
                    infos[i].storage=valores4;
                    int valr_i = v_cont*i;
                    infos[i].inicio=valr_i;                    
                    if (i==num_threads-1)                   
                        infos[i].fim=largura;
                    else
                        infos[i].fim=(i+1)*v_cont;
                    pthread_t v = val[i];
                    pthread_create(&v,NULL,calcularXY_pthrd,(void*)&infos[i]);
                    
                }  
                for(int i=0;i<num_threads;i++){
                    pthread_t v = val[i];
                    pthread_join(v,NULL);
                }
                end_time=clock();
            }    
            if(modo==1){
                start_time=clock();
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
                        valores1[i1*largura + i2] = (int)valor;            
                    }
                end_time=clock();                    
            }if(modo==2){
                start_time=clock();
                #pragma omp parallel num_threads(num_threads)
                {
                    #pragma omp parallel for    
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
                        valores2[i1*largura + i2] = (int)valor;            
                        }
                    } 
                    end_time=clock(); 
            }if(modo==3){
                start_time=clock();
                int valor = num_threads/2;
                #pragma omp parallel num_threads(valor)
                { 
                    #pragma omp parallel for
                    for(int i=0;i<valor;i++){
                        infos[i].x_p=x_p; 
                        infos[i].total=total;
                        infos[i].larg = largura;
                        infos[i].y_p=y_p;
                        infos[i].i1=i1;
                        infos[i].storage=valores3;
                        int valr_i = v_cont*i;
                        infos[i].inicio=valr_i;                    
                        if (i==valor-1)                   
                            infos[i].fim=largura;
                        else
                            infos[i].fim=(i+1)*v_cont;
                        pthread_t v = val[i];
                        pthread_create(&v,NULL,calcularXY_pthrd,(void*)&infos[i]);
                    
                    }  
                    for(int i=0;i<valor;i++){
                        pthread_t v = val[i];
                        pthread_join(v,NULL);
                    }
            }
            end_time=clock();
        }
        tempos[modo-1]=(double)(end_time-start_time)/CLOCKS_PER_SEC;
        if (modo==1)
            abrirarquivo(modo,valores1,largura,altura);
        if (modo==2)
            abrirarquivo(modo,valores2,largura,altura);
        if (modo==3)
            abrirarquivo(modo,valores3,largura,altura);
        if (modo==4)
            abrirarquivo(modo,valores4,largura,altura);
        }
    }

    
    
    abrirarquivo(TIME_WRITE,)


    free(valores1);
    free(valores4);
    free(valores2);
    free(valores3);
    return 0;
}