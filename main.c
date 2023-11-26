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
#endif

#include "filas.h"
#include "procs.h"

#define NIVEISP 2
#define QUANTUM 3
#define CHANCE 4
#define min(a, b) (((a) < (b)) ? (a) : (b))

enum {DISCO, IMPRESSORA, FITAMAG};

void showStatus(int, Processo *, FilaProcs **, FilaProcs **);
void trocaProcesso(Processo **, FilaProcs **);
void executa(Processo **, int);

int main(void)
{
	int i;
	int tempo;
	int timeSlice = 0;
	int nProc;
	FilaProcs *Q[NIVEISP + 1];
	FilaProcs *IO[4];

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
		if (!(rand() % CHANCE) && nProc < 10)  {
			inserir(novoProc(tempo), Q[0]);
			nProc++;
		}

		if(timeSlice == QUANTUM || atual == NULL) {
			trocaProcesso(&atual, Q);
			timeSlice = 0;
		}

		showStatus(tempo, atual, Q, IO);
		executa(&atual, tempo);
		
		tempo++;
		timeSlice++;
		sleep(1);
	}


	return 0;
}

void showStatus(int t, Processo *atual, FilaProcs **prioridades, FilaProcs **ios)
{
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

void executa(Processo **executando, int t)
{
	Processo *proc = *executando;
	if(proc != NULL) {
		if (proc->tempoExec == proc->tempoTotal) {
			printf("Processo de PID %d terminou com turnaround de %d unidades de tempo\n", proc->PID, t - proc->tempoInicio);
			free(*executando);
			*executando = NULL;
			return;
		}
		
		(*executando)->tempoExec++;
	}
}