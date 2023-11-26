#include <stdlib.h>
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
