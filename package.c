/*
 * Bryant Conquest
 * bmconquest@crimson.ua.edu
 */

#ifdef DEBUG
#endif

#include <signal.h>
#include <stdio.h>
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

struct mesg_buffer {
  long mesg_type;
  char mesg_text[100];
} message;

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
  int id = (int)"bmconquest";

  key = ftok("./bmconquest", id);
  printf("2\n");
  msgid = msgget(key, 0666 | IPC_CREAT);
  printf("3\n");
  message.mesg_type = 1;

  printf("Write Data : ");
  gets(message.mesg_text);
  printf("4\n");
  msgsnd(msgid, &message, sizeof(message), 0);

  if(DEBUG) {
    printf("key->%d\n",key);
  }

	destroyMatrix(matrix1);
	destroyMatrix(matrix2);
	return 0;
}
