#include<pthread.h>
#include<stdio.h>
#include<stdlib.h>
#include<semaphore.h>

#define NUM_THREAD_INFECT 	3
#define NUM_THREAD_LAB 		3

#define VACINA 		1
#define VIRUS_MORTO 2
#define INSUMO 		3

typedef struct operacoes
{	
	int quantOperacaoMinima;
	int quantOperacaoLab[NUM_THREAD_LAB];
	int quantOperacaoInfec[NUM_THREAD_INFECT];
	
	sem_t *semaph_lab;
	sem_t *semaph_infec;
	pthread_mutex_t *mutex_op;
	
}T_operacoes,*P_operacoes;

typedef struct laboratorio{

	int ident;
	int produtoEmPosse_1;
	int produtoEmPosse_2;
	int quantOperacaoMinima;
	
	//CRIT
	int statusProduto_1;
	int statusProduto_2;
	
	P_operacoes operacoes;
	

}T_laboratorio,*P_laboratorio;

typedef struct infectado
{
	int ident;
	int produtoEmPosse;
	
	P_laboratorio mesa[NUM_THREAD_LAB];
	P_operacoes operacoes;
	
	
}T_infectado,*P_infectado;

pthread_mutex_t mutex;
sem_t semaph_cont0;
sem_t semaph_cont1;

void f_inicOperacoes(
	P_operacoes op,
	int quantMinima,
	pthread_mutex_t mutex_op,
	sem_t semaph_lab,
	sem_t semaph_infec){

	op->quantOperacaoMinima = quantMinima;

	for(int cont = 0; cont < (NUM_THREAD_LAB);cont++){

		op->quantOperacaoLab[cont] = 0;
	}

	for(int cont = 0; cont < (NUM_THREAD_INFECT);cont++){

		op->quantOperacaoInfec[cont] = 0;
	}

	op->mutex_op 		= &mutex;
	op->semaph_lab 		= &semaph_cont1;
	op->semaph_infec 	= &semaph_cont0;

}

void f_iniciaListaLaboratorio(P_laboratorio lista[],P_operacoes op){
	
	int cont = 0;

	while(cont < NUM_THREAD_LAB){

		P_laboratorio lab;

		lab = (P_laboratorio)malloc(1*sizeof(T_laboratorio));

		if(cont == 0){

			lab->ident = cont;
			lab->produtoEmPosse_1 = VACINA;
			lab->produtoEmPosse_2 = INSUMO;
			lab->statusProduto_1 = 0;
			lab->statusProduto_2 = 0;
		
			lab->operacoes = op;

		}else if (cont == (NUM_THREAD_LAB - 1)){

			lab->ident = cont;
			lab->produtoEmPosse_1 = INSUMO;
			lab->produtoEmPosse_2 = VIRUS_MORTO;
			lab->statusProduto_1 = 0;
			lab->statusProduto_2 = 0;

			lab->operacoes = op;

		}else{

			lab->ident = cont;
			lab->produtoEmPosse_1 = VIRUS_MORTO;
			lab->produtoEmPosse_2 = VACINA;
			lab->statusProduto_1 = 0;
			lab->statusProduto_2 = 0;

			lab->operacoes = op;

		}

		lista[cont] = lab;
		cont++;

	}
}

void f_iniciaListaInfectado(P_infectado lista[],P_laboratorio labs[],P_operacoes op){

	int cont = 0;

	while(cont < NUM_THREAD_INFECT){

		P_infectado infec;

		infec = (P_infectado)malloc(1*sizeof(T_infectado));

		if(cont == 0){

			infec->ident = cont;
			infec->produtoEmPosse = VACINA;
			
			infec->mesa[0] = labs[0];
			infec->mesa[1] = labs[1];
			infec->mesa[2] = labs[2];

			infec->operacoes = op;
	

		}else if (cont == (NUM_THREAD_INFECT - 1)){

			infec->ident = cont;
			infec->produtoEmPosse = INSUMO;
			
			infec->mesa[0] = labs[0];
			infec->mesa[1] = labs[1];
			infec->mesa[2] = labs[2];

			infec->operacoes = op;

		}else{

			infec->ident = cont;
			infec->produtoEmPosse = VIRUS_MORTO;
			
			infec->mesa[0] = labs[0];
			infec->mesa[1] = labs[1];
			infec->mesa[2] = labs[2];

			infec->operacoes = op;

		}

		lista[cont] = infec;
		cont++;

	}
}

int verificaQuantidadeMinimaEmTodos(P_operacoes op){


	for(int cont = 0; cont < (NUM_THREAD_LAB);cont++){

		if(op->quantOperacaoLab[cont] < op->quantOperacaoMinima){
			return 0;
		}
	}

	for(int cont = 0; cont < (NUM_THREAD_INFECT);cont++){

		if(op->quantOperacaoInfec[cont] < op->quantOperacaoMinima){
			return 0;
		}
	}

	return 1;

}

void *f_threadLab(void* param){

	P_laboratorio lab = (P_laboratorio)param;
	
	while(1){
		
		
		sem_wait(lab->operacoes->semaph_lab);
		
		pthread_mutex_lock(lab->operacoes->mutex_op);
		
		//Todos antingiram o minimo?
		if(verificaQuantidadeMinimaEmTodos(lab->operacoes)){
			return 0;
		}
		pthread_mutex_unlock(lab->operacoes->mutex_op);

		int cont = 0;

		//Verifica seu estoque
		pthread_mutex_lock(lab->operacoes->mutex_op);
		
		if(lab->statusProduto_1 == 1 || lab->statusProduto_2 == 1){
			lab->statusProduto_1 = 0;
			lab->statusProduto_2 = 0;
			lab->operacoes->quantOperacaoLab[lab->ident] +=1;
			
		}
		
		pthread_mutex_unlock(lab->operacoes->mutex_op);
		
		sem_post(lab->operacoes->semaph_infec);
		
	};
	
	return 0;
}

void *f_threadInfect(void* param){

	P_infectado infec = (P_infectado)param;

	int produtoEmPosse = infec->produtoEmPosse;
	
	while(1){

		pthread_mutex_lock(infec->operacoes->mutex_op);
		
		//Todos antingiram o minimo?
		if(verificaQuantidadeMinimaEmTodos(infec->operacoes)){
			return 0;
		}
		
		pthread_mutex_unlock(infec->operacoes->mutex_op);

		//Quantidade Ingredientes encotrados
		int restanteEncontrado 	= 0;

		//Produto jah consumido
		int jahConsumido 		= -1;

		sem_post(infec->operacoes->semaph_lab);
		
		pthread_mutex_lock(infec->operacoes->mutex_op);
		for (int cont = 0; cont < NUM_THREAD_LAB;cont++){
			
			if(restanteEncontrado < 2){
				
				
				//Verificar disponibilidade do produto
				if(	infec->mesa[cont]->statusProduto_1 != 1 ||
					infec->mesa[cont]->statusProduto_2 != 1 ){

					//Produto em posse eh diferente do produto 1 eo jahconsumido e esta disponivel?
					if(	produtoEmPosse != infec->mesa[cont]->produtoEmPosse_1 &&
						jahConsumido != infec->mesa[cont]->produtoEmPosse_1 &&
						infec->mesa[cont]->statusProduto_1 == 0){

						//Att. indisponibilidade 
						infec->mesa[cont]->statusProduto_1 = 1;

						//Jah consumido
						jahConsumido = infec->mesa[cont]->produtoEmPosse_1;

						//Add um ingrediente
						restanteEncontrado++;

						
					}
					//Produto em posse 	eh diferente do produto 2 e esta disponivel?
					if(	produtoEmPosse != infec->mesa[cont]->produtoEmPosse_2 &&
						jahConsumido != infec->mesa[cont]->produtoEmPosse_2 && 
						infec->mesa[cont]->statusProduto_2 == 0){


						//Att. indisponibilidade 
						infec->mesa[cont]->statusProduto_2 = 1;

						//Jah consumido
						jahConsumido = infec->mesa[cont]->produtoEmPosse_2;

						//Add um ingrediente
						restanteEncontrado++;
						
					}

				}
				
			}
		
		}

		if(restanteEncontrado == 2){
			
			restanteEncontrado = 0;
			jahConsumido = -1;
			infec->operacoes->quantOperacaoInfec[infec->ident] +=1;
		}		

		pthread_mutex_unlock(infec->operacoes->mutex_op);

		sem_wait(infec->operacoes->semaph_infec);

	}
	
	return 0;

}

void printOperacoes(P_operacoes op){

	for(int cont =0;cont < NUM_THREAD_LAB;cont++){
		printf("Laboratorio %d 	- %d vezes\n",cont,op->quantOperacaoLab[cont]);
	}

	for(int cont =0;cont < NUM_THREAD_INFECT;cont++){
		printf("Infectado %d 	- %d vezes\n",cont,op->quantOperacaoInfec[cont]);
	}

	
}


int main(int argc, char **argv){


	int total_thread = NUM_THREAD_INFECT + NUM_THREAD_INFECT;
	int retorno_thread;
	int quantOperacaoMinima = atoi(argv[1]);

	pthread_mutex_t mutex_op;
	sem_t semaph_lab;
	sem_t semaph_infec;

	//Produtos para producao da Vacina	
	int produtosVacina[3] = {
		VACINA,
		VIRUS_MORTO,
		INSUMO};

	P_laboratorio	listaLab[NUM_THREAD_LAB];
	P_infectado 	listaInfec[NUM_THREAD_INFECT];
	P_operacoes op;
	
	op = (P_operacoes)malloc(1*sizeof(T_operacoes));
	
	f_inicOperacoes(op,quantOperacaoMinima,mutex_op,semaph_lab,semaph_infec);
	f_iniciaListaLaboratorio(listaLab,op);
	f_iniciaListaInfectado(listaInfec,listaLab,op);

	sem_init(op->semaph_infec,0,0);
	sem_init(op->semaph_lab,0,0);

	//Cria vetor thread
    pthread_t * vetor_threads;
    vetor_threads = malloc(total_thread*sizeof(pthread_t));


	//Cria thread
    for(long cont = 0; cont < total_thread;cont++){

    	// Monta primeiro os lab
    	if(cont < NUM_THREAD_LAB){
    		retorno_thread = pthread_create(&vetor_threads[cont],NULL,f_threadLab,(void*)listaLab[cont]);
    	}else{
    		retorno_thread = pthread_create(&vetor_threads[cont],NULL,f_threadInfect,(void*)listaInfec[cont-NUM_THREAD_LAB]);
    	}
           
        if(retorno_thread != 0){
            
            printf("Error CREATE");
            exit(-1);
        }    
    }

    int condicaoParada = 1;
    
    while(condicaoParada){
    	condicaoParada = !verificaQuantidadeMinimaEmTodos(op);
    }

    printOperacoes(op);

    free(vetor_threads);

    sem_destroy(op->semaph_lab);
    sem_destroy(op->semaph_infec);
    

}