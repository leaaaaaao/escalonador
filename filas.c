#include <stdlib.h>
#include <stdio.h>
#include "filas.h"
#include "procs.h"

#define MEMCHECK(X) if((X) == NULL) {fprintf(stderr, "ERRO: Sem memoria\n"); exit(1);}

#define TAM 10

FilaProcs *novaFila(void)
{
	FilaProcs *fila;
	
	fila = malloc(sizeof(*fila));
	MEMCHECK(fila);
	fila->vetor = malloc(sizeof(*fila->vetor) * TAM);
	MEMCHECK(fila->vetor);

	fila->tam = TAM;
	fila->inicio = -1;
	fila->fim = -1;

	return fila;
}

void inserir(Processo *proc, FilaProcs *fila)
{
	if(fila->inicio == -1) {
		fila->vetor[0] = proc;
		fila->inicio = 0;
		fila->fim = 1;
		return;
	}
	if(fila->inicio == fila->fim)
		overflowFila(fila);

	fila->vetor[fila->fim++] = proc;
	fila->fim %= fila->tam;

	return;
} 

void overflowFila(FilaProcs *fila)
{
	Processo **novoVet;
	int i;

	novoVet = malloc(sizeof(*novoVet) * fila->tam * 2);
	MEMCHECK(novoVet);
	
	for(i = 0; i < fila->tam; i++)
		novoVet[i] = fila->vetor[(fila->inicio + i) % fila->tam];

	free(fila->vetor);
	fila->vetor = novoVet;
	fila->inicio = 0;
	fila->fim = fila->tam;
	fila->tam *= 2;

	return;	
}

int vazia(FilaProcs *fila)
{
	if(fila->inicio == -1)
		return 1;
	return 0;
}

Processo *removerPrimeiro(FilaProcs *fila)
{
	Processo *antigo;

	if(vazia(fila)) {
		fprintf(stderr, "ERRO: Tentando remover de fila de processos vazia\n");
		exit(2);
	}

	antigo = fila->vetor[fila->inicio];
	fila->inicio++;
	fila->inicio %= fila->tam;

	if (fila->fim == fila->inicio) {
		fila->inicio = -1;
		fila->fim = -1;
	}

	return antigo;
}

void printFila(FilaProcs *fila)
{
	int i;
	int nElem;

	if (fila->inicio == -1) {
		puts("");
		return;
	}

	nElem = fila->fim - fila->inicio;
	if (nElem <= 0)
		nElem += fila->tam;

	for (i = 0; i < nElem; i++)
		printf("%d ", fila->vetor[(fila->inicio + i) % fila->tam]->PID);
	puts("");
}