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

int main(int argc, char *argv[])
{
	key_t key;
	int msgid;
	int id = 65;
		
	//signal(SIGINT, sig_handler);
	
	if (argc > 3 || argc < 2) {
		printf("usage is \"compute <thread pool size> <-n for just read and output"
           "calculations>\"\n");
		exit(1);
	}

	key = ftok("./bmconquest", id);
	printf("key -> %d\n",key);

  int threadLimit = atoi(argv[1]); printf("%d\n", threadLimit);
	int limit = 0;
	while(1) {
		msgid = msgget(key, 0666 | IPC_CREAT);
		msg a;
		msgrcv(msgid, &a, sizeof(a), 1, 0);
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
    msg *write = malloc(sizeof(msg));
    write->type = 2;
    write->jobid = sent++;
    write->rowvec = a.rowvec;
    write->colvec = a.colvec;
    write->innerDim = p;
    int size = (4 * sizeof(int));
    int rc = msgsnd(msgid, write, size, 0);
    printf("\nSending job %4d type %ld size %d (rc=%d)\n",
        write->jobid, write->type, size, rc);
    free(write);
		limit += 1;
	}
	printf("SAh");
	msgctl(msgid, IPC_RMID, NULL);
	return 0;
}
