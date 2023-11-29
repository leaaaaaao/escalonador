#ifndef PROCS_H
#define PROCS_H
#define MAXLOGS 50

#include<stdio.h>

#define MEMCHECK(X) if((X) == NULL) {fprintf(stderr, "ERRO: Sem memoria\n"); exit(1);}

typedef struct {
	int PID;
	int PPID;
	int prioridade;		/* Número da fila de prioridade */
	int tempoInicio; 	/* u.t em que o processo foi admitido */
	int tempoExec; 		/* Tempo de execução decorrido */
	int tempoTotal;		/* Tempo que o processo precisa passar no processador para encerrar */
	int chanceIO; 		/* Inverso da chance do processo pedir IO a cada unidade de tempo em execução */
} Processo;

typedef struct {
	int PID;
	int chegada;
	int saida;
	int turnaround;
	int nIOs;			/* Número de IOs pedidos durante a execução */
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