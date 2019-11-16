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

  matrixStruct *a;
  matrixStruct *b;

  a = initMatrix(argv[1]);
  b = initMatrix(argv[2]);

  if (DEBUG) {
    printf("(R: %2d\tC:%2d)X(R: %2d\tC:%2d)\n",
           a->rows, a->cols, b->rows, b->cols);
    printf("\nMatrix 1\n");
    printMatrix(a);
    printf("\nMatrix 2\n");
    printMatrix(b);
  }
  return 0;
}
