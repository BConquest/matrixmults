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
pthread_mutex_t readLock;
pthread_mutex_t outLock;

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
  int **out;
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
  int i = 0;
  int size, rc;

  packageArgsStruct *tempArgs = (packageArgsStruct *)args;
  
  matrixStruct *c = tempArgs->a;
  matrixStruct *d = tempArgs->b;

  msg read;
  msg *write = malloc(sizeof(msg));

  write->type = 1;
  write->jobid = tempArgs->id;
  write->rowvec = tempArgs->row;
  write->colvec = tempArgs->col;
  write->innerDim = c->cols;

  int *row = getRow(c, tempArgs->row);
  int *col = getCol(d, tempArgs->col);
   
  for (i = 0; i < c->cols; i++) {
    write->data[i] = row[i];
    write->data[c->cols+i] = col[i];
    if (DEBUG) printf("(%dx%d)\t",row[i],col[i]);
  }
  if (DEBUG) printf("\n");
  
  size = (4 * sizeof(int)) + (2 * c->cols * sizeof(int));
  pthread_mutex_lock(&wLock);
  rc = msgsnd(msgid, write, size, 0);
  pthread_mutex_unlock(&wLock);

  if (rc < 0) {
    perror("Error in msg send");
    exit(1);
  }

  printf("Sending job id %4d type %ld size %d (rc=%d)\n",
         write->jobid, write->type, size, rc);

  pthread_mutex_lock(&readLock);
  rc = msgrcv(msgid, &read, sizeof(msg), 2, 0);
  

  if (rc < 0) {
    perror("Error in message recieve");
    exit(1);
  }

  pthread_mutex_lock(&srLock);
  recvJobs++;
  pthread_mutex_unlock(&srLock);

  size = (4 * sizeof(int));
  printf("Receiving Job id %d type %ld size %d\n"
         , read.jobid, read.type, size);
 // if (DEBUG) {
    printf("\n\t\t\t\t\t\t%d\n", read.innerDim);
 // }
  pthread_mutex_lock(&outLock);
  tempArgs->out[read.rowvec][read.colvec] = read.innerDim;
  pthread_mutex_unlock(&outLock);
  pthread_mutex_unlock(&readLock);

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
  int **outputMatrix;
  key_t key;
  pthread_t *threads;

  signal(SIGINT, sig_handler);
  pthread_mutex_init(&srLock, NULL);
  pthread_mutex_init(&wLock,NULL);
  pthread_mutex_init(&readLock, NULL);
  pthread_mutex_init(&outLock, NULL);

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

  outputMatrix = allocateMatrix(a->rows,b->cols);

  if (!threads) {
    perror("Error Allocating Space for threads");
    exit(1);
  }
  
  for (i = 0; i < a->rows; i++) {
    for (j = 0; j < b->cols; j++) {
      packageArgsStruct *args = malloc(sizeof(packageArgsStruct));
      sleep(sleepTime);
      if (!args) {
        perror("Error Allocating space for thread arguments");
        exit(1);
      }
      args->a = a;
      args->b = b;
      args->out = outputMatrix;
      args->row = i;
      args->col = j;
      pthread_mutex_lock(&srLock);
      args->id = sentJobs++;
      pthread_mutex_unlock(&srLock);
      pthread_create(&(threads[thread++]),NULL,package,(void *)args);
    }
  }
 
  for (i = 0; i < thread; i++) {
    pthread_join(threads[i], NULL);
  }
  
  if (DEBUG) {
    printf("Total jobs sent:     %d\n", sentJobs);
    printf("Total jobs recieved: %d\n", recvJobs);
  }
  
  for (i = 0; i < a->rows; i++) {
    for (j = 0; j < b->cols; j++) {
      printf("{%d,%d}%d ", i, j, outputMatrix[i][j]);
    }
    printf("\n");
  }
  
  writeToFile(argv[3], outputMatrix, a->rows, b->cols); 

  for (i = 0; i < a->rows; i++) {
    free(outputMatrix[i]);
  }
  free(outputMatrix);

  free(threads);
  destroyMatrix(a);  
  destroyMatrix(b);
  return 0;
}
