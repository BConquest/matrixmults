/*
 * Bryant Conquest
 * bmconquest@crimson.ua.edu
 */

#ifdef DEBUG
#endif

#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#include "./include/matrix.h"

volatile int sent = 0;
volatile int recv = 0;
pthread_mutex_t sentLock;

typedef struct QueueMessage {
	long type;
	int jobid;
	int rowvec;
	int colvec;
	int innerDim;
	int data[100];
} msg;

typedef struct packageArgument {
	matrix *a;
	matrix *b;
	key_t key;
	int row;
	int col;
} packageArgument;

void *packager(void *packArg)
{
	matrix *c = ((packageArgument *)packArg)->a;
	matrix *d = ((packageArgument *)packArg)->b;

	key_t pkey = ((packageArgument *)packArg)->key;
	int msgid = msgget(pkey, 0666 | IPC_CREAT);
	msg a;

	a.type = 1;
	pthread_mutex_lock(&sentLock);
	a.jobid = sent++;
	pthread_mutex_unlock(&sentLock);
	a.rowvec = ((packageArgument *)packArg)->row;
	a.colvec = ((packageArgument *)packArg)->col;
	a.innerDim = d->c;

	int *row = getRow(c, a.rowvec);
	int *col = getCol(d, a.colvec);

	for (int k = 0; k < a.innerDim; k++) {
		if (k < c->c) a.data[k] = row[k];
		if (DEBUG && k < c->c) 
			printf("%d ", a.data[k]);
	}
	if (DEBUG) printf("\t");

	for (int l = 0; l < a.innerDim; l++) {
		if (l < d->r) a.data[l+a.innerDim] = col[l];
		if (DEBUG && l < d->r)
			printf("%d ", a.data[l+a.innerDim]);
	}
	if (DEBUG) printf("\n");
	
	int rc = msgsnd(msgid, &a, sizeof(a), 0);
	printf("Sending job id %04d type %d size %ld (rc=%d)\n"
			, a.jobid, 1, sizeof(a), rc);

	free(row);
	free(col);
	return NULL;
}

void sig_handler(int signo)
{
	if (signo == SIGINT) {
		pthread_mutex_lock(&sentLock);
		printf("Jobs Sent %d Jobs Received %d\n", sent, recv);
		pthread_mutex_unlock(&sentLock);
	}
}


int main(int argc, char *argv[])
{
	int sleepTime = atoi(argv[4]);

	pthread_mutex_init(&sentLock, NULL);
	signal(SIGINT, sig_handler);

	if(DEBUG) {
		printf("Debug mode is on.\n");
	}

	if (argc != 5) {
		printf("Usage is ./package <matrix 1 filename> <matrix 2 filename>"
				" <output matrix data file> <secs between thread creation>\n");
		exit(1);
	}

	matrix *matrix1 = initMatrix(argv[1]);
	matrix *matrix2 = initMatrix(argv[2]);

	checkMatrix(matrix1, matrix2);
	key_t key;
	int id = 65;

	key = ftok("./bmconquest", id);

	if (DEBUG) {
		printf("key->%d\n", key);
		printf("Num Jobs: %04d\n", matrix1->r*matrix2->c);
	}

	pthread_t threads[matrix1->r*matrix2->c];
	int threadCounter = 0;

	for (int i = 0; i < matrix1->r; i++) {
		for (int j = 0; j < matrix2->c; j++) {
			sleep(sleepTime);
			packageArgument threadArgument;
			threadArgument.a = matrix1;
			threadArgument.b = matrix2;
			threadArgument.key = key;
			threadArgument.row = i;
			threadArgument.col = j;
			pthread_create(&threads[threadCounter++], NULL, packager, (void *)&threadArgument);
		}
	}
	for (int i = 0; i < matrix1->r*matrix2->c; i++) {
		int test = pthread_join(threads[i],NULL);
		if (DEBUG) printf("rc(%d) -> %d\n", i, test);
	}

	destroyMatrix(matrix1);
	destroyMatrix(matrix2);
	return 0;
}
