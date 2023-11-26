#ifndef FILAS_H
#define FILAS_H
#include "procs.h"

typedef struct {
	Processo **vetor;
	int tam;
	int inicio;
	int fim;
} FilaProcs;

FilaProcs *novaFila(void);
void inserir(Processo *, FilaProcs *);
void overflowFila(FilaProcs *);
int vazia(FilaProcs *);
Processo *removerPrimeiro(FilaProcs *);
void printFila(FilaProcs *);

#endif
