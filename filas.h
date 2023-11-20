#ifndef FILAS_H
#define FILAS_H

typedef struct {
	int PID;
	/**TODO**/
} Processo;

typedef struct {
	Processo **vetor;
	int tam;
	int inicio;
	int fim;
} FilaProcs;

Processo *novoProc(void); /* Dependendo de como a struct ficar, vai precisar de um apagarProc() */
FilaProcs *novaFila(void); /* Mesma coisa */

void inserir(Processo *, FilaProcs *);
void overflowFila(FilaProcs *);
int vazia(FilaProcs *);
void removerPrimeiro(FilaProcs *);

#endif
