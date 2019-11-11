#ifndef __matrix_h__
#define __matrix_h__

#include <stdio.h>

#include "./error.h"

typedef struct matrix {
  int r;
  int c;
  int *pmatrix;
} matrix;

int *allocateMatrix(int, int);
int *getRow(matrix *, int);

matrix *initMatrix(char *);

void checkMatrix(matrix *, matrix *);
void printMatrix(int *, int, int);

#endif
