#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <pthread.h>
#include <time.h>
#include <string.h>

typedef struct dados{
    double x_p; 
    int larg;
    double total;
    double y_p;
    int i1;
    int inicio;
    int val;
    int fim;
    int *storage;
}dados;

typedef struct dados3{
    int larg;
    int alt;
    int *storage;
}dados3;

void *limparVetor(void *arg){
    dados3* info = (dados3*)arg;
    int l=(info->larg);
    int a = (info->alt);
    for(int i=0;i<(l*a);i++){
        info->storage[i]=0;
    }
}


void *calcularNovo(void *arg){
    dados *info = (dados *)arg;
    for(int i2=(info->val);i2<(info->fim);i2+=(info->inicio)){
        for(int i3=0;i3<info->larg;i3++){
            double x=0.0;
            double y=0.0;
            double c_real = -2.0+i3*(info->x_p);
            double c_imag = -1.5+(i2)*(info->y_p);
            double interacoes =0.0;
                
            while(((x*x)+(y*y))<=4.0 && interacoes<(info->total)){
                double x_temp = (x*x) - (y*y) + c_real; 
                y= 2*x*y +(c_imag);
                x= x_temp;
                interacoes++;
            }
            double valor = ((255.0*interacoes)/info->total);
            info->storage[(i2)*(info->larg) + i3] = (int)valor;            
        }  
    }
}


void *calcularXYPthrd(void *arg){
    dados *info = (dados *)arg;
    for(int i2=(info->inicio);i2<info->fim;i2++){
        double x=0.0;
        double y=0.0;
        double c_real = -2.0+i2*(info->x_p);
        double c_imag = -1.5+(info->i1)*(info->y_p);
        double interacoes =0.0;
                
        while(((x*x)+(y*y))<=4.0 && interacoes<(info->total)){
                double x_temp = (x*x) -  (y*y) + c_real; 
                y= 2*x*y +(c_imag);
                x= x_temp;
                interacoes++;
        }
        double valor = ((255.0*interacoes)/info->total);
        info->storage[(info->i1)*(info->larg) + i2] = (int)valor;            
    }  
}

void abrirTempo(long double *tempo){
    FILE *arq;
    arq=fopen("times.txt","w"); 
    if(arq!=NULL){
            for(int i=0;i<4;i++){
                if(i==0)
                    fprintf(arq,"Serial: %Lf\n",tempo[i]);
                if (i==1)
                    fprintf(arq,"OpenMP: %Lf\n",tempo[i]);
                if (i ==2)
                    fprintf(arq,"Pthreads1: %Lf\n",tempo[i]);
                if (i==3)
                    fprintf(arq,"Pthreads2: %Lf\n",tempo[i]);
            }
    }
    fclose(arq);
}

void abrirArquivo(int mode, int *valores,int larg, int altura){
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
                    fprintf(arq,"%d ",valores[cont*larg+cont2]);
                }
            fprintf(arq,"\n");
        }
    }
    fclose(arq);

}   

int main(int argc, char *argv[]){
    if(argc<6){
        return -1;
    }

    if (strcmp(argv[1],"mandelbrot")==0 && argc==6){
    int largura = atoi(argv[2]);
    int altura = atoi(argv[3]);
    if(largura<=0 || altura<=0){
        return -1;
    }
    int num_threads = atoi(argv[5]);
    double total = atoi(argv[4]);
    if(total<=0.0 || num_threads<=0){
        return -1;
    }
    double x1 = -2.0, x2=1.0;
    double y1 = -1.5,y2=1.5; 
    clock_t  start_time,end_time;
    long double tempos[4];
    
    int *valores1 = (int *) malloc(sizeof(int)*largura*altura);
    int *valores2 = (int *) malloc(sizeof(int)*largura*altura);
    int *valores3 =  (int *) malloc(sizeof(int)*largura*altura);
    int *valores4 = (int *) malloc(sizeof(int)*largura*altura);
    for(int modo=1;modo<=4;modo++){
        double Z=0.0;
        double x_p =(x2-x1)/largura; 
        double y_p = (y2-y1)/altura;
        dados infos[num_threads];
        //dados infos3[num_threads];
        int v_cont = largura/num_threads;  
        pthread_t val[num_threads];
        pthread_t val3[num_threads];

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
                    pthread_create(&v,NULL,calcularXYPthrd,(void*)&infos[i]);
                    
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
                dados3 valor[num_threads];
                for(int i=0;i<num_threads;i++){
                    valor[i].storage = valores3;
                    valor[i].alt=altura;
                    valor[i].larg=largura;
                    pthread_create(&val3[i],NULL,limparVetor,(void*)&valor[i]);
                }
                for(int i=0;i<num_threads;i++){
                    pthread_join(val3[i],NULL);
                }

                /*for(int i=0;i<num_threads;i++){
                    infos3[i].x_p=x_p; 
                    infos3[i].total=total;
                    infos3[i].larg = largura;
                    infos3[i].y_p=y_p;
                    infos3[i].i1=i1;
                    infos3[i].storage=valores3;
                    infos3[i].val=i;

                    infos3[i].inicio=num_threads;                    
                    infos3[i].fim=altura;
                    pthread_t v = val[i];
                    pthread_create(&val[i],NULL,calcularNovo,(void*)&infos3[i]);
                }
                for(int i=0;i<num_threads;i++){
                    pthread_t v = val[i];
                    pthread_join(val[i],NULL);
                }
                */
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
                        valores3[i1*largura + i2] = (int)valor;            
                    }

                end_time=clock();
        }
        tempos[modo-1]=(double)(end_time-start_time)/CLOCKS_PER_SEC;
        if (modo==1)
            abrirArquivo(modo,valores1,largura,altura);
        if (modo==2)
            abrirArquivo(modo,valores2,largura,altura);
        if (modo==3)
            abrirArquivo(modo,valores3,largura,altura);
        if (modo==4)
            abrirArquivo(modo,valores4,largura,altura);
        }
    }
    abrirTempo(tempos);

    free(valores1);
    free(valores4);
    free(valores2);
    free(valores3);

    
    }   
    return 0;
}