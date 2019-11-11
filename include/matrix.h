#ifndef __matrix_h__
#define __matrix_h__

#include <stdio.h>

#include "./error.h"

typedef struct matrix {
  int r;
  int c;
  int *pmatrix;
} matrix;

/*
 * allocateMatrix
 * int Row
 * int Col
 * Returns a pointer to a 1d array that is large enough to store a Rows*Cols
 * array
 * If failing to allocate space program exits
 */
int *allocateMatrix(int, int);

/*
 * getRow
 * matrix *a
 * int row
 * Returns all of the values in the Row of the matrix passed in
 * if passed in an invalid row returns 0
 */
int *getRow(matrix *, int);

/*
 * getCol
 * matrix a
 * int row
 * Returns all of the values in the col of the matrix pass in
 * if passed in an invalid row returns 0
 */
int *getCol(matrix *, int);

/*
 * initMatrix
 * char *filename
 * creates a matrix from the passed in filename
 * if failed to open file program exits
 */
matrix *initMatrix(char *);

/*
 * checkMatrix
 * matrix *a
 * matrix *b
 * checks to make sure that both matrix a and b are within the size
 * requirements and can be added to each other
 */
void checkMatrix(matrix *, matrix *);

/*
 * printMatrix
 * int *array
 * int row
 * int col
 * can print a matrix from the passed in 1d array
 */
void printMatrix(int *, int, int);

/*
 * destroyMatrix
 * int *matrix
 * frees all memory allocated to a matrix data structre
 */
void destroyMatrix(matrix *);

#endif
