#ifndef PROCS_H
#define PROCS_H
#define MAXLOGS 50

#include<stdio.h>

#define MEMCHECK(X) if((X) == NULL) {fprintf(stderr, "ERRO: Sem memoria\n"); exit(1);}

typedef struct {
	int PID;
	int PPID;
	int prioridade;
	int tempoInicio;
	int tempoExec; /* Tempo que ele já passou no processador */
	int tempoTotal;
	int chanceIO;
} Processo;

typedef struct {
	int PID;
	int chegada;
	int saida;
	int turnaround;
	int nIOs;
	int tiposIO[15];
} Registro;

typedef struct {
	char textos[MAXLOGS][128];
	int chegada[MAXLOGS];
	int inicio;
	int fim;
} Log;


Processo *novoProc(int); 
void initLog(Log *);
void printRegistros (Registro *, int);

#endif