#ifdef DEBUG
#endif

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <unistd.h>

#include "./include/matrix.h"

int msgid;
int sentJobs = 0;
int recvJobs = 0;

pthread_mutex_t srLock;
pthread_mutex_t wLock;

typedef struct QueueMessage{
  long type;
  int jobid;
  int rowvec;
  int colvec;
  int innerDim;
  int data[100];
} msg;

typedef struct packageArgsStruct {
  matrixStruct *a;
  matrixStruct *b;
  int row;
  int col;
  int id;
} packageArgsStruct;

void sig_handler(int signo)
{
  pthread_mutex_lock(&srLock);
  if (DEBUG) printf("SIGNAL CATCHED: %d\t", signo);
  printf("Jobs Sent %d Jobs Received %d\n", sentJobs, recvJobs);
  pthread_mutex_unlock(&srLock);
}

void *package(void *args)
{
  packageArgsStruct *tempArgs = (packageArgsStruct *)args;
  
  matrixStruct *c = tempArgs->a;
  matrixStruct *d = tempArgs->b;

  msg *write = malloc(sizeof(msg));

  write->type = 1;
  write->jobid = tempArgs->id;
  write->rowvec = tempArgs->row;
  write->colvec = tempArgs->col;
  write->innerDim = c->cols;

  int *row = getRow(c, tempArgs->row);
  int *col = getCol(d, tempArgs->col);
   
  for (int i = 0; i < c->cols; i++) {
    write->data[i] = row[i];
    write->data[c->cols+i] = col[i];
    if (DEBUG) printf("(%dx%d)\t",row[i],col[i]);
  }
  if (DEBUG) printf("\n");
  
  int size = (4 * sizeof(int)) + (2 * c->cols * sizeof(int));
  pthread_mutex_lock(&wLock);
  int rc = msgsnd(msgid, write, size, 0);
  pthread_mutex_unlock(&wLock);

  if (rc < 0) {
    perror("Error in msg send");
    exit(1);
  }

  printf("Sending job id %4d type %ld size %d (rc=%d)\n",
         write->jobid, write->type, size, rc);

  free(write);
  free(row);
  free(col);
  free(tempArgs);
  pthread_exit(0);
}

int main(int argc, char **argv)
{
  matrixStruct *a, *b;
  int thread = 0, sleepTime;
  int i, j;
  key_t key;
  pthread_t *threads;

  signal(SIGINT, sig_handler);

  if (argc != 5) {
    printf("Usage is: ./package <file1> <file2> <output file> <secs to sleep"
           "between thread creation>.\n");
    exit(1);
  }

  a = initMatrix(argv[1]);
  b = initMatrix(argv[2]);
  
  checkMatrix(a, b);
  
  sleepTime = atoi(argv[4]);

  if (DEBUG) {
    printf("Matrix A: %d X %d\n", a->rows, a->cols);
	  printMatrix(a);
    printf("Matrix B: %d X %d\n", b->rows, b->cols);
    printMatrix(b);
  }

  key = ftok("./bmconquest", 65);
  msgid = msgget(key, 0666 | IPC_CREAT);

  threads = malloc(a->rows*b->cols*sizeof(pthread_t));

  if (!threads) {
    perror("Error Allocating Space for threads");
    exit(1);
  }
  
  for (i = 0; i < a->rows; i++) {
    for (j = 0; j < b->cols; j++) {
      packageArgsStruct *args = malloc(sizeof(packageArgsStruct));
      if (!args) {
        perror("Error Allocating space for thread arguments");
        exit(1);
      }
      args->a = a;
      args->b = b;
      args->row = i;
      args->col = j;
      pthread_mutex_lock(&srLock);
      args->id = sentJobs++;
      pthread_mutex_unlock(&srLock);
      pthread_create(&(threads[thread++]),NULL,package,(void *)args);
      sleep(sleepTime);
    }
  }
 
  for (i = 0; i < thread; i++) {
    pthread_join(threads[i], NULL);
  }
  
  if (DEBUG) {
    printf("Total jobs sent:     %d\n", sentJobs);
    printf("Total jobs recieved: %d\n", recvJobs);
  }

  free(threads);
  destroyMatrix(a);  
  destroyMatrix(b);
  return 0;
}
