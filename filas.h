#ifndef FILAS_H
#define FILAS_H
typedef struct {
	int PID;
	int PPID;
	int prioridade;
	int tempoInicio;
	int tempoExec;
	int tempoTotal;
	/**TODO**/
} Processo;

typedef struct {
	Processo **vetor;
	int tam;
	int inicio;
	int fim;
} FilaProcs;

Processo *novoProc(int); /* Dependendo de como a struct ficar, vai precisar de um apagarProc() */
FilaProcs *novaFila(void); /* Mesma coisa */

void inserir(Processo *, FilaProcs *);
void overflowFila(FilaProcs *);
int vazia(FilaProcs *);
Processo *removerPrimeiro(FilaProcs *);
void printFila(FilaProcs *);

#endif
