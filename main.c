#include<stdio.h>
#include<stdlib.h>
#include<time.h>

#ifdef __linux__
#include <unistd.h>
void clear(void) {
	system("clear");
}
#endif

#ifdef _WIN32
#include <windows.h>
void clear(void) {
	system("cls");
}

#define sleep(X) Sleep(X)
#endif

#include "filas.h"
#include "procs.h"

#define NIVEISP 2
#define QUANTUM 3
#define CHANCENOVO 4
#define MAXPROC 10
#define TICK 1

#define min(a, b) (((a) < (b)) ? (a) : (b))

enum {DISCO, IMPRESSORA, FITAMAG};

void showStatus(int, Processo *, FilaProcs **, FilaProcs **);
void trocaProcesso(Processo **, FilaProcs **);
void executa(Processo **, int, FilaProcs **);
void rodaIO(FilaProcs **, FilaProcs **);
int acabou(FilaProcs **, FilaProcs **);

int main(void)
{
	int i;
	int tempo;
	int utGastas = 0;
	int nProc;
	FilaProcs *Q[NIVEISP + 1];
	FilaProcs *IO[4];
	FilaProcs **iterador;

	Processo *atual = NULL;
	
	srand(time(NULL));
	for (i = 0; i < NIVEISP; i++)
		Q[i] = novaFila();
	for (i = 0; i < 3; i++)
		IO[i] = novaFila();
	Q[NIVEISP] = NULL;
	IO[3] = NULL;

	tempo = 0;
	nProc = 0;
	while(1) {
		if (nProc == MAXPROC && acabou(Q, IO)) {
			puts("Encerrando...");
			return 0;
		}

		if (!(rand() % CHANCENOVO) && nProc < MAXPROC)  {
			inserir(novoProc(tempo), Q[0]);
			nProc++;
		}

		rodaIO(IO, Q);

		if(utGastas == QUANTUM || atual == NULL) {
			trocaProcesso(&atual, Q);
			utGastas = 0;
		}

		showStatus(tempo, atual, Q, IO);
		executa(&atual, tempo, IO);

		tempo++;
		utGastas++;
		sleep(TICK);
	}


	return 0;
}

void showStatus(int t, Processo *atual, FilaProcs **prioridades, FilaProcs **ios)
{
	static char *nomesIOS[3] = {"disco", "impressora", "fita magnetica"};
	int n;

	n = 0;
	clear();
	printf("Tempo: %d\n", t);
	if (atual == NULL)
		puts("Nenhum processo sendo executado agora");
	else
		printf("Executando processo de PID %d\n", atual->PID);

	while (*prioridades != NULL) {
		printf("Fila de prioridade %d: ", n);
		printFila(*prioridades);
		n++;
		prioridades++;
	}
	puts("");
	
	n = 0;
	while (*ios != NULL) {
		printf("Fila do IO de tipo %s: ", nomesIOS[n]);
		printFila(*ios);
		n++;
		ios++;
	}

	return;
}

void trocaProcesso(Processo **atual, FilaProcs **filas)
{
	Processo *tmp;
	FilaProcs **it;

	it = filas;
	while (*it != NULL) {
		if (!vazia(*it)) {
			tmp = *atual;
			*atual = removerPrimeiro(*it);
			if(tmp != NULL) {
				tmp->prioridade = min(tmp->prioridade + 1, NIVEISP - 1);
				inserir(tmp, filas[tmp->prioridade]);
			}
			break;
		}
		it++;
	}

	return;
}

void executa(Processo **executando, int t, FilaProcs **ios)
{
	Processo *proc = *executando;
	static char *nomesIOS[3] = {"disco", "impressora", "fita magnetica"};
	int tipoIO;

	if(proc != NULL) {
		(*executando)->tempoExec++;
		if (proc->tempoExec == proc->tempoTotal) {
			printf("\nProcesso de PID %d terminou com turnaround de %d unidades de tempo\n", proc->PID, t - proc->tempoInicio);
			free(*executando);
			*executando = NULL;
			return;
		}
		if (!(rand() % proc->chanceIO)) {
			*executando = NULL;
			tipoIO = rand() % 3;
			printf("\nProcesso de PID %d solicitou IO do tipo %s e foi para a fila correspondente\n", proc->PID, nomesIOS[tipoIO]);
			inserir(proc, ios[tipoIO]); /* Tipo de IO aleatório */
			return;
		}
	}
}

void rodaIO(FilaProcs **ios, FilaProcs **prioridades)
{
	int tempo[3] = {3, 5, 7}; /* Tempo para esse tipo de IO acabar */
	static int tempoRestante[3] = {3, 5, 7};
	int i;

	for (i = 0; i < 3; i++) {
		if(!vazia(ios[i]) && --tempoRestante[i] == 0){
			inserir(removerPrimeiro(ios[i]), prioridades[(i == DISCO) ? 1 : 0]);
			tempoRestante[i] = tempo[i];
		}
	}

	return;
}

int acabou(FilaProcs **p, FilaProcs **io)
{
	int rodando = 0;
	while (*p != NULL) {
		if (!vazia(*p))
			rodando = 1;
		p++;
	}
	while (*io != NULL) {
		if (!vazia(*io))
			rodando = 1;
		io++;
	}
	return !rodando;
}