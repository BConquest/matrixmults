/*
 * Bryant Conquest
 * bmconquest@crimson.ua.edu
 */

#ifdef DEBUG
#endif

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "./include/error.h"

typedef struct matrix {
  int r;
  int c;
  int *pmatrix;
} matrix;

int *allocateMatrix(int, int);
matrix *initMatrix(char *);
void checkMatrix(matrix *, matrix *);
void printMatrix(int *, int, int);

int main(int argc, char *argv[])
{
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

  free(matrix1->pmatrix);
  free(matrix2->pmatrix);
  free(matrix1);
  free(matrix2);
  return 0;
}

void checkMatrix(matrix *a, matrix *b)
{
  if (a->c != b->r) {
    Ferror("Matrixs do not correlate");
  }
  if (a->r > 50 || a->c > 50) {
    Ferror("Matrix a is too large");
  }
  if (b->r > 50 || b->c > 50) {
    Ferror("Matrix b is too large");
  }
}

void printMatrix(int *ptr, int rows, int cols)
{
  int i;
  
  for (i = 0; i < rows*cols; i++) {
      if (i % cols == 0) {
        printf("\n");
      } 
      printf("%d ", ptr[i]);
  }
  printf("\n");
}

int *allocateMatrix(int rows, int cols)
{
  int* ptr;
  int i;

  ptr = malloc((rows*cols)*sizeof(int));
  
  if(!ptr) {
    Ferror("Cannot allocate space");
  }

  for (i = 0; i < rows*cols; i++) {
      ptr[i] = 0;
  }

  return ptr;
}

matrix *initMatrix(char *filename) {
  FILE *fileptr = fopen(filename, "r");
  int temp_rows, temp_cols;
  int *temp_matrix;

  if (!fileptr) {
    printf("Error opening <%s>", filename);
    Ferror("Error");
  }

  fscanf(fileptr, "%d", &temp_rows);
  fscanf(fileptr, "%d", &temp_cols);
  if (DEBUG)
    printf("matrix dimensions r: %d c: %d\n", temp_rows, temp_cols);
  
  temp_matrix = allocateMatrix(temp_rows, temp_cols);
  for (int i = 0; i < temp_rows*temp_cols; i++)
    fscanf(fileptr, "%d", &temp_matrix[i]);
  
  if(DEBUG)
    printMatrix(temp_matrix, temp_rows, temp_cols);

  matrix *matrix1 = malloc(sizeof(matrix));
  matrix1->r = temp_rows;
  matrix1->c = temp_cols;
  matrix1->pmatrix = temp_matrix;

  fclose(fileptr);
  return matrix1;
}
