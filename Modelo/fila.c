#include<stdio.h>
#include<stdlib.h>

typedef struct exemplo{

	int ident;

}T_Exemplo,*P_Exemplo;


typedef struct 
{
	int limite;
	P_Exemplo dado;
	int primeiro;
	int ultimo;
	int quantItens;

}T_fila,*P_fila;


void f_inicFila(P_fila fila, int limite){

	fila -> limite = limite;
	fila -> dado = (P_Exemplo) malloc(10*sizeof(T_Exemplo));
	fila -> primeiro = 0;
	fila -> ultimo = -1;
	fila -> quantItens = 0;

}

void f_inserirFila(P_fila fila,P_Exemplo exemplo){

	if(fila->ultimo == fila->limite-1){

		fila->ultimo = -1;
	}

	fila-> ultimo++;
	fila-> dado[fila->ultimo] = *(P_Exemplo) exemplo;
	fila-> quantItens++;
}

int f_estaVazia(P_fila fila){

	return(fila->quantItens == 0);
}

int f_estaCheia(P_fila fila){

	return(fila->quantItens == fila->limite);
}

void f_printFila(P_fila fila){

	int cont,i;

	for(int cont = 0,i = fila->primeiro;cont < fila->quantItens;cont++){

		printf("%d\n",fila->dado[i++].ident);

		if(i == fila->limite){
			i = 0;
		}
	}

}

T_Exemplo f_remover(P_fila fila){

	T_Exemplo aux = fila->dado[fila->primeiro++];

	if(fila->primeiro == fila->limite){
		fila->primeiro == 0;
	}

	fila->quantItens--;
	return aux;
}

int main(int argc, char **argv){

	P_fila fila;

	P_Exemplo exemplo;
	

	exemplo = (P_Exemplo) malloc(1*sizeof(T_Exemplo)); 
	fila 	= (P_fila)malloc(1*sizeof(T_fila));
	
	
	f_inicFila(fila,10);
	
	exemplo->ident = 99;
	f_inserirFila(fila,exemplo);
	exemplo->ident = 88;
	f_inserirFila(fila,exemplo);
	exemplo->ident = 77;
	f_inserirFila(fila,exemplo);
	f_printFila(fila);

	printf("Apos f_remover()\n");
	
	f_remover(fila);
	f_printFila(fila);


}