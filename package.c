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
  int sleepTime = atoi(argv[4]);

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
	key_t key;
	int msgid;
	int id = 65;

	key = ftok("./bmconquest", id);

	if (DEBUG) {
		printf("key->%d\n", key);
		printf("Num Jobs: %04d\n", matrix1->r*matrix2->c);
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
      
      for (int m = 0; m < 100; m++) {
        a.data[m] = 0;
      }

			for (int k = 0; k < 50; k++) {
				if (k < matrix1->c) {
					a.data[0+k] = row[k];
				} else {
					a.data[0+k] = 0;
				}
				if (DEBUG && k < matrix1->c) {
					printf("%d ", a.data[0+k]);
				}
			}
			if (DEBUG) printf("\t");

			for (int l = 0; l < 50; l++) {
				if (l < matrix2->r) {
					a.data[50+l] = col[l];
				} else {
					a.data[50+l] = 0;
				}
				if (DEBUG && l < matrix2->r) {
					printf("%d ", a.data[50+l]);
				}
			}
			if(DEBUG) printf("\n");
			int rc = msgsnd(msgid, &a, sizeof(a), 0);
			printf("Sending job id %04d type %d size %ld (rc=%d)\n", sent, 1, sizeof(a), rc);
      
      sleep(sleepTime);

      free(row);
      free(col);
		}
	}

	destroyMatrix(matrix1);
	destroyMatrix(matrix2);
	return 0;
}
