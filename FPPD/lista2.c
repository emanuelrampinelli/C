#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>

#define NUM_THREAD 10
#define TAM_VETOR 100000


int vetor[TAM_VETOR];
int quant_soma_thread[NUM_THREAD];

int index_global    = 0;
int result_soma     = 0;

pthread_mutex_t mutex1;

void *f_somaThread(void* param){

    int index_local = 0;
    int result_soma_local = 0;

    long ident_thread = (long)param;

    quant_soma_thread[ident_thread] = 0;

    do{

        //CRIT - Index atual
        pthread_mutex_lock(&mutex1);
        index_local = index_global;
        index_global++;
        pthread_mutex_unlock(&mutex1);

        if(index_local < TAM_VETOR){
            result_soma_local += vetor[index_local];
            quant_soma_thread[ident_thread]++;
        }


    }while(index_local < TAM_VETOR);

    //CRIT - Soma
    pthread_mutex_lock(&mutex1);
    result_soma += result_soma_local;
    pthread_mutex_unlock(&mutex1);

    return 0;
}

float porcentagemSoma(int posicao){

    return (float)(100*quant_soma_thread[posicao])/(float)TAM_VETOR;
}

int main(int argc, char **argv){

    //retorno da funcao
    int retorno_thread;

    //cria vetor thread
    pthread_t * vetor_threads;
    vetor_threads = malloc(NUM_THREAD*sizeof(pthread_t));


    //popula vetor de inteiros
    for(int i = 0; i < TAM_VETOR;i++){

        vetor[i] = i+1;
    }

    //Cria thread
    for(long cont = 0; cont < NUM_THREAD;cont++){

        retorno_thread = pthread_create(&vetor_threads[cont],NULL,f_somaThread,(void*)cont);
            
        if(retorno_thread != 0){
            
            printf("Error CREATE");
            exit(-1);
        }    
    }

    for(int cont = 0; cont < NUM_THREAD;cont++){
        
        retorno_thread = pthread_join(vetor_threads[cont],NULL);

        if(retorno_thread != 0){
            
            printf("Error JOIN");
            exit(-2);
        }
    }


    for(int cont = 0;cont < NUM_THREAD;cont++){

        printf("O trabalho de thread #%d foi de %d somas |%.2f %%|\n",
            cont,
            quant_soma_thread[cont],
            porcentagemSoma(cont));

    }

    printf("Soma total eh %d\n",result_soma);

    free(vetor_threads);

}


