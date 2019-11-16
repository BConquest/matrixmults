#ifdef DEBUG
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./include/matrix.h"

int main(int argc, char **argv)
{
  if (argc != 5) {
    printf("Usage is: ./package <file1> <file2> <output file> <secs to sleep"
           "between thread creation>.\n");
    exit(1);
  }

  matrixStruct *a, *b;
  a = initMatrix(argv[1]);
  b = initMatrix(argv[2]);
  
  checkMatrix(a, b);
  
  if (DEBUG) {
    printf("Matrix A: %d X %d\n", a->rows, a->cols);
	  printMatrix(a);
    printf("Matrix B: %d X %d\n", b->rows, b->cols);
    printMatrix(b);
  }

  destroyMatrix(a);  
  destroyMatrix(b);
  return 0;
}
