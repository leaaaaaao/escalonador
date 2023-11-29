#include <stdlib.h>
#include <string.h>
#include "procs.h"

Processo *novoProc(int t)
{
	static int NPID = 0;
	Processo *novo;

	novo = malloc(sizeof(*novo));
	MEMCHECK(novo);
	novo->PID = NPID++;
	novo->PPID = novo->PID;
	novo->prioridade = 0;

	novo->tempoTotal = rand() % 15;
	novo->tempoTotal++;
    novo->tempoExec = 0;
	novo->tempoInicio = t;

	novo->chanceIO = 2 + (rand() % 10);

	return novo;
}

void initLog(Log *l)
{
	l->inicio = 0;
	l->fim = 0;
}

void printRegistros (Registro *hist, int num)
{
	int i;
	int j;
	Registro r;

	puts("\nPID | Chegada | Saida | Turnaround | IOs");
	puts("________________________________________");
	for (i = 0; i < num; i++) {
		r = hist[i];
		printf("% 3d | % 7d | % 5d | % 10d | % 3d\n", r.PID, r.chegada, r.saida, r.turnaround, r.nIOs);
		puts("________________________________________");
	}
	puts("");
}