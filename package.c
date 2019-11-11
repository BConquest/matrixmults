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

#include "./include/error.h"
#include "./include/matrix.h"

volatile int sent;
volatile int recv;

void sig_handler(int signo) {
	if (signo == SIGINT)
		printf("Jobs Sent %d Jobs Received %d\n", sent, recv);
}

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
	
	destroyMatrix(matrix1);
	destroyMatrix(matrix2);
	while(1) sleep(1);
	return 0;
}
