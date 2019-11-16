#ifndef __matrix_h__
#define __matrix_h__

#include <stdio.h>
#include <stdlib.h>

/*
 * rows & cols are indexed 0
 * pmatrix is a 2d-allocated array
 */
typedef struct matrixStruct {
  int rows;
  int cols;
  int **pmatrix;
} matrixStruct;

/*
 * dotProduct
 * int *row
 * int *col
 * int multi
 * Returns the dot products of row and col
 */
int dotProduct(int *, int *, int);

/*
 * allocateMatrix
 * int Row
 * int Col
 * Returns a pointer to a 1d array that is large enough to store a Rows*Cols
 * array
 * If failing to allocate space program exits
 */
int **allocateMatrix(int, int);

/*
 * getRow
 * matrix *a
 * int row
 * Returns all of the values in the Row of the matrix passed in
 * if passed in an invalid row program exits
 */
int *getRow(matrixStruct *, int);

/*
 * getCol
 * matrix a
 * int row
 * Returns all of the values in the col of the matrix pass in
 * if fails exits the programs
 */
int *getCol(matrixStruct *, int);

/*
 * initMatrix
 * char *filename
 * creates a matrix from the passed in filename
 * if failed to open file program exits
 */
matrixStruct *initMatrix(char *);

/*
 * checkMatrix
 * matrix *a
 * matrix *b
 * checks to make sure that both matrix a and b are within the size
 * requirements and can be added to each other
 */
void checkMatrix(matrixStruct *, matrixStruct *);

/*
 * printMatrix
 * int *matrix
 * can print a matrix from the passed in array
 */
void printMatrix(matrixStruct *);

/*
 * destroyMatrix
 * int *matrix
 * frees all memory allocated to a matrix data structre
 */
void destroyMatrix(matrixStruct *);

#endif
