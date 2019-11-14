#include <stdio.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>

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

int main(void) {
	key_t key;
	int msgid;
	int id = 65;

	key = ftok("./bmconquest", id);
	printf("key -> %d\n",key);
	while(1) {
		msgid = msgget(key, 0666 | IPC_CREAT);
		msg a;
		msgrcv(msgid, &a, sizeof(a), 1, 0);
		printf("type %ld jobid %d rowvec %d colvec %d innerDim %d\n",
				a.type, a.jobid, a.rowvec, a.colvec, a.innerDim);
		for(int i = 0; i < a.innerDim; i++) {
			printf("%d ", a.data[i]);
		}
		printf("\n");
		for(int j = 0; j < a.innerDim; j++) {
			printf("%d ", a.data[50+j]);
		}
		printf("\n");
	}

	msgctl(msgid, IPC_RMID, NULL);
	return 0;
}
