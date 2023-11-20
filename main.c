#include<stdio.h>
#include<stdlib.h>
#include "filas.h"

#define NTEST 20

void printEstadoFila(FilaProcs *fila)
{
	int i;
	int nElem;
	
	if(fila->inicio == -1)
		nElem = 0;
	else {
		nElem = fila->fim - fila->inicio;
		if (nElem <= 0)
			nElem = fila->tam + nElem;
	}

	for(i = 0; i < nElem; i++)
		printf("%d ", fila->vetor[(fila->inicio +i) % fila->tam]->PID);
	printf("\nTamanho: %d\nInicio: %d\nFim: %d\n", fila->tam, fila->inicio, fila->fim);

	return;
}

int main(void)
{
	Processo *p[NTEST];
	FilaProcs *fila;
	int i;

	fila = novaFila();
	printf("Estado da fila:\nTamanho: %d\nInicio: %d\nFim: %d\n\n", fila->tam, fila->inicio, fila->fim);
	for(i = 0; i < NTEST; i++) {
		p[i] = novoProc();
		inserir(p[i], fila);
		printEstadoFila(fila);
	}

	for(i = 0; i < NTEST; i++) {
		removerPrimeiro(fila);
		printEstadoFila(fila);
	}

	return 0;
}
