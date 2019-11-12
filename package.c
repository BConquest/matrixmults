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

#include "./include/error.h"
#include "./include/matrix.h"

volatile int sent;
volatile int recv;

void sig_handler(int signo) {
	if (signo == SIGINT)
		printf("Jobs Sent %d Jobs Received %d\n", sent, recv);
}

typedef struct QueueMessage {
	long type;
	int jobid;
	int rowvec;
	int colvec;
	int innerDim;
	int data[100];
} msg;

int main(int argc, char *argv[])
{
	signal(SIGINT, sig_handler);

	if(DEBUG) {
		printf("Debug mode is on.\n");
	}

	if (argc != 5) {
		printf("Usage is ./package <matrix 1 filename> <matrix 2 filename>"
				" <output matrix data file> <secs between thread creation>\n");

		Ferror("Check Arguments");
	}

	matrix *matrix1 = initMatrix(argv[1]);
	matrix *matrix2 = initMatrix(argv[2]);

	checkMatrix(matrix1, matrix2);
	printf("1\n");
	key_t key;
	int msgid;
	int id = 65;

	key = ftok("./bmconquest", id);

	if (DEBUG) {
		printf("key->%d\n", key);
		printf("Num Jobs: %d\n", matrix1->r*matrix2->c);
	}

	for (int i = 0; i < matrix1->r; i++) {
		for (int j = 0; j < matrix2->c; j++) {
			msgid = msgget(key, 0666 | IPC_CREAT);
			msg a;
			a.type = 1;
			a.jobid = sent;
			sent += 1;
			a.rowvec = i;
			a.colvec = j;
			a.innerDim = matrix1->c;
			
			int *row = getRow(matrix1, i);
			int *col = getCol(matrix2, j);

			for (int k = 0; k < matrix1->c; k++)
			{
				a.data[0+k] = row[k];
				if (DEBUG) {
					printf("%d ", a.data[0+k]);
				}
			}
			printf("\t");

			for (int l = 0; l < matrix2->r; l++)
			{
				a.data[51+l] = col[l];
				if (DEBUG) {
					printf("%d ", a.data[51+l]);
				}
			}
			printf("\n");
			msgsnd(msgid, &a, sizeof(a), 0);
		}
	}

	destroyMatrix(matrix1);
	destroyMatrix(matrix2);
	return 0;
}
