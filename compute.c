#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <pthread.h>

#include "./include/matrix.h"

typedef struct QueueMessage{
	long type;
	int jobid;
	int rowvec;
	int colvec;
	int innerDim;
	int data[100];
} msg;


volatile int sent = 0;
volatile int recv = 0;

void sig_handler(int signo)
{
	if (signo == SIGINT) {
		printf("Jobs Sent %d Jobs Received %d\n", sent, recv);
	}
}

typedef struct {
	int *row;
	int *col;
	int mult;
} argument;

void *multiply(void *arg)
{
	argument *a = (argument *)arg;
	int* r = a->row;
	int* c = a->col;
	int mult = a->mult;

	int *product = malloc(sizeof(int));
	
	if (!product) {
		perror("Not enough memory for dot product allocations\n");
		exit(1);
	}

	int index = 1;

	for (index = 1; index <= mult; index++) {
		product += r[index-1] * c[index-1];
	}

	if (DEBUG) printf("DOT PRODUCT: %d", *product);
	pthread_exit(product);
}
	

int main(int argc, char *argv[])
{
	key_t key;
	int msgid;
	int id = 65;
	signal(SIGINT, sig_handler);

	if (argc > 3 || argc < 2) {
		printf("usage is \"compute <thread pool size> <-n for just read and output calculations>\"");
		exit(1);
	}
	
	int THREAD_POOL_SIZE = atoi(argv[2]);

	key = ftok("./bmconquest", id);
	printf("key -> %d\n",key);
	while(1) {
		msgid = msgget(key, 0666 | IPC_CREAT);
		msg a;
		msgrcv(msgid, &a, sizeof(a), 1, 0);
		sent += 1;
		printf("type %ld jobid %d rowvec %d colvec %d innerDim %d\n",
				a.type, a.jobid, a.rowvec, a.colvec, a.innerDim);
		int *row = malloc(a.innerDim*sizeof(int));
		int *col = malloc(a.innerDim*sizeof(int));
		for(int i = 0; i < a.innerDim; i++) {
			if(DEBUG) printf("%d ", a.data[i]);
			row[i] = a.data[i];
		}
		if (DEBUG) printf("\t");
		for(int j = 0; j < a.innerDim; j++) {
			if(DEBUG) printf("%d ", a.data[50+j]);
			col[j] = a.data[50+j];
		}
		int dProduct = dotProduct(row, col, a.innerDim);
		if (DEBUG) printf("=%d\n", dProduct);
	}

	msgctl(msgid, IPC_RMID, NULL);
	return 0;
}
