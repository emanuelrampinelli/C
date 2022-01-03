#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>

#define NUM_THREAD 5

void *f_thread(void * param){

	long id_thread = (long) param;

	printf("Executando thread - %ld\n",id_thread);

	return 0;
}

int main(int argc, char **argv){


	int quant_thread = NUM_THREAD;

	pthread_t * v_threads;

	v_threads = malloc(quant_thread*sizeof(pthread_t));

	for(long cont = 0; cont < quant_thread;cont++){

		printf("Criando thread %ld\n",cont);

		if(pthread_create(&v_threads[cont],NULL,f_thread,(void*)cont) != 0){
			
			printf("Error CREATE");
			exit(-1);
		}	
	}

	for(int cont = 0; cont < quant_thread;cont++){
		
		if(pthread_join(v_threads[cont],NULL) != 0){
			
			printf("Error JOIN");
			exit(-2);
		}
	}

	free(v_threads);

}