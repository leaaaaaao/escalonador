#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#ifdef __linux__
#include <unistd.h>
#endif
#ifdef _WIN32
#include <windows.h>
#endif

#include "filas.h"

#define NIVEISP 3
#define QUANTUM 3
#define min(a, b) (((a) < (b)) ? (a) : (b))

enum {DISCO, IMPRESSORA, FITAMAG};

void showStatus(int t, Processo *atual, FilaProcs **prioridades, FilaProcs **ios)
{
	int n;

	n = 0;
	system("clear");
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

int main(void)
{
	int i;
	int tempo;
	int timeSlice = 0;
	int nProc;
	FilaProcs *Q[NIVEISP + 1];
	FilaProcs *IO[4];

	Processo *atual = NULL;
	Processo *tmp;
	
	srand(time(NULL));
	for (i = 0; i < NIVEISP; i++)
		Q[i] = novaFila();
	Q[NIVEISP] = NULL;
	IO[3] = NULL;

	tempo = 0;
	while(1) {
		if (tempo % 2 && !(rand() % 3))  {
			tmp = novoProc(tempo);
			nProc = tmp->PID;
			if (nProc == 40) {
				puts("Processo 40 chegou, encerrando");
				break;
			}
			inserir(tmp, Q[0]);
		}

		if(timeSlice == QUANTUM || atual == NULL) {
			trocaProcesso(&atual, Q);
			timeSlice = 0;
		}

		showStatus(tempo, atual, Q, IO);	
		if(atual != NULL) {
			atual->tempoExec++;
			if (atual->tempoExec == atual->tempoTotal) {
				printf("Processo de PID %d terminou com turnaround de %d unidades de tempo\n", atual->PID, tempo - atual->tempoInicio);
				free(atual);
				atual = NULL;
			}
		}
		tempo++;
		timeSlice++;
		sleep(1);
	}


	return 0;
}
