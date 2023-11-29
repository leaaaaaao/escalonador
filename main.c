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

#define sleep(X) Sleep(X * 1000)
#endif

#include "filas.h"
#include "procs.h"

#define NIVEISP 2 		/* Número de filas de prioridade */
#define TICK 1    		/* Duração em segundos de uma unidade de tempo*/
#define QUANTUM 3 		/* Quantum em unidades de tempo (ticks) */
#define CHANCENOVO 4	/* Chance de um processo novo surgir a cada u.t. */
#define MAXPROC 10		/* Número total de processos que vão aparecer do início ao fim do programa */
#define TEMPOLOG 5		/* Número de 'u.t.'s que os logs ṕermanecem na tela. */

#define min(a, b) (((a) < (b)) ? (a) : (b))

enum {DISCO, IMPRESSORA, FITAMAG};

void showStatus(int, Processo *, FilaProcs **, FilaProcs **, Log *);
void trocaProcesso(Processo **, FilaProcs **);
void executa(Processo **, int, FilaProcs **, Registro *, Log *);
void rodaIO(FilaProcs **, FilaProcs **); /* Atualiza os tempos restantes de IO e suas respectivas filas */
int acabou(FilaProcs **, FilaProcs **); /* Verifica se as filas estão todas vazias*/

int main(void)
{
	int i;
	int tempo;			/* Usado como relogio */
	int utGastas = 0;	/* Unidades de tempo que o processo atualmente em execução já gastou do quantum */
	int nProc;			/* Número de processos já criados */

	Log logs;
	initLog(&logs);

	FilaProcs *Q[NIVEISP + 1];
	FilaProcs *IO[4];
	Registro historico[MAXPROC];

	Processo *atual = NULL;
	Processo *tmp = NULL;
	
	srand(time(NULL));
	for (i = 0; i < NIVEISP; i++)
		Q[i] = novaFila();
	for (i = 0; i < 3; i++)
		IO[i] = novaFila();
	Q[NIVEISP] = NULL;
	IO[3] = NULL;

	for (i = 0; i < MAXPROC; i++)
		historico[i].nIOs = 0;

	tempo = 0;
	nProc = 0;
	while(1) {
		if (atual == NULL && nProc == MAXPROC && acabou(Q, IO)) {
			puts("Encerrando...");
			break;
		}

		if (!(rand() % CHANCENOVO) && nProc < MAXPROC)  {
			tmp = novoProc(tempo);
			historico[tmp->PID].chegada = tempo;
			historico[tmp->PID].PID = tmp->PID;
			inserir(tmp, Q[0]);
			nProc++;
		}

		rodaIO(IO, Q);

		if(utGastas == QUANTUM || atual == NULL) {
			trocaProcesso(&atual, Q);
			utGastas = 0;
		}

		showStatus(tempo, atual, Q, IO, &logs);
		tempo++;
		utGastas++;
		executa(&atual, tempo, IO, historico, &logs);

		sleep(TICK);
	}
	

	printRegistros(historico, MAXPROC);

	return 0;
}

void showStatus(int t, Processo *atual, FilaProcs **prioridades, FilaProcs **ios, Log *logs)
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

	for(n = logs->inicio; n < logs->fim; n++) {
		if(t - logs->chegada[n] > TEMPOLOG) {
			logs->inicio = (logs->inicio + 1) % MAXLOGS;
			continue;
		}
		printf("%s", logs->textos[n]);
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

void executa(Processo **executando, int t, FilaProcs **ios, Registro *hist, Log *logs)
{
	Processo *proc = *executando;
	static char *nomesIOS[3] = {"disco", "impressora", "fita magnetica"};
	int tipoIO;

	if(proc != NULL) {
		(*executando)->tempoExec++;
		if (proc->tempoExec == proc->tempoTotal) {
			hist[proc->PID].saida = t;
			hist[proc->PID].turnaround = t - proc->tempoInicio;

			sprintf(logs->textos[logs->fim], "\nProcesso de PID %d terminou com turnaround de %d unidades de tempo\n", proc->PID, hist[proc->PID].turnaround);
			logs->chegada[logs->fim++] = t;
			logs->fim %= MAXLOGS;

			free(*executando);
			*executando = NULL;
			return;
		}
		if (!(rand() % proc->chanceIO)) {
			*executando = NULL;
			tipoIO = rand() % 3;

			sprintf(logs->textos[logs->fim], "\nProcesso de PID %d solicitou IO do tipo %s e foi para a fila correspondente\n", proc->PID, nomesIOS[tipoIO]);
			logs->chegada[logs->fim++] = t;
			logs->fim %= MAXLOGS;

			hist[proc->PID].tiposIO[hist[proc->PID].nIOs++] = tipoIO;
			
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