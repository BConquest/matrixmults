#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <pthread.h>

#include "./include/matrix.h"

pthread_mutex_t readLock;
pthread_mutex_t writeLock;
pthread_mutex_t srLock;

int sent = 0;
int recv = 0;
int msgid = 0;

typedef struct QueueMessage{
	long type;
	int jobid;
	int rowvec;
	int colvec;
	int innerDim;
	int data[100];
} msg;

void sig_handler(int signo)
{
	if (signo == SIGINT) {
		printf("Jobs Sent %d Jobs Received %d\n", sent, recv);
	}
}

void *computer()
{
		msg a;
		pthread_mutex_lock(&readLock);
		msgrcv(msgid, &a, sizeof(a), 1, 0);
		pthread_mutex_unlock(&readLock);
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
			if(DEBUG) printf("%d ", a.data[a.innerDim+j]);
			col[j] = a.data[a.innerDim+j];
		}
    		
		int p = dotProduct(row, col, a.innerDim);
    if (DEBUG) printf("R: %d * C: %d = %d ", a.rowvec, a.colvec, p);
		free(row);
		free(col);
		pthread_mutex_lock(&writeLock);
    msg *writeFile = malloc(sizeof(msg));
    writeFile->type = 2;
    writeFile->jobid = a.jobid;
    writeFile->rowvec = a.rowvec;
    writeFile->colvec = a.colvec;
    writeFile->innerDim = p;
    int size = (4 * sizeof(int));
    int rc = msgsnd(msgid, writeFile, size, 0);
		pthread_mutex_unlock(&writeLock);
    printf("\nSending job %4d type %ld size %d (rc=%d)\n",
        writeFile->jobid, writeFile->type, size, rc);
    free(writeFile);
		pthread_exit(0);
}

int main(int argc, char *argv[])
{
	key_t key;
		
	signal(SIGINT, sig_handler);
	pthread_mutex_init(&readLock, NULL);
	pthread_mutex_init(&writeLock, NULL);
	pthread_mutex_init(&srLock, NULL);
	
	if (argc > 3 || argc < 2) {
		printf("usage is \"compute <thread pool size> <-n for just read and output"
           "calculations>\"\n");
		exit(1);
	}

	key = ftok("./bmconquest", 65);
	printf("key -> %d\n",key);

  int threadLimit = atoi(argv[1]); printf("%d\n", threadLimit);
	pthread_t threads[threadLimit];
	msgid = msgget(key, 0666 | IPC_CREAT);
	int counter = 0;
	while(1) {
		pthread_create(&threads[counter++], NULL, computer, 0);
		if (counter >= threadLimit) break;
	}
	for (int i = 0; i < counter;i++) {
		pthread_join(threads[i], NULL);
	}
	return 0;
}
