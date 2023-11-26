#ifndef PROCS_H
#define PROCS_H
#include<stdio.h>

#define MEMCHECK(X) if((X) == NULL) {fprintf(stderr, "ERRO: Sem memoria\n"); exit(1);}

typedef struct {
	int PID;
	int PPID;
	int prioridade;
	int tempoInicio;
	int tempoExec;
	int tempoTotal;
	int chanceIO;
} Processo;

Processo *novoProc(int); 

#endif