#include "../include/matrix.h"

int dotProduct(int *row, int *col, int mult)
{
	int product = 0, index = 1;

	for (index = 1; index <= mult; index++) {
		product += row[index-1] * col[index-1];
	}
	printf("\t%d\t", product);
	return product;
}

int **allocateMatrix(int rows, int cols)
{
  int i;

  int **arr = (int **)malloc(rows * sizeof(int *));
  
  if (!arr) {
    perror("Error Allocating Space for matrix");
    exit(1);
  }

  for (i = 0; i < rows; i++) {
    arr[i] = (int *) malloc(cols * sizeof(int));
    if (!arr[i]) {
      perror("Error Allocating Sub-Space for matrix");
      exit(1);
    }
  }

  return arr;
}
/*
int *getCol(matrix *a, int col)
{
}

int *getRow(matrix *a, int row)
{
}
*/
matrixStruct *initMatrix(char *filename)
{
  FILE *fileptr;
  int rows, cols;
  int i, j;

  fileptr = fopen(filename, "r");
  
  if (!fileptr) {
    fprintf(stderr, "Can't open file <%s>\n", filename);
    perror("Reson is in stderr");
    exit(1);
  }
  
  fscanf(fileptr, "%d %d\n", &rows, &cols);
  
  if (rows > 50 || cols > 50) {
    fprintf(stderr, "Error: matrix file <%s> contains dimensions to large\n",
            filename);
    exit(1);
  }

  int **matrix = allocateMatrix(rows, cols);
  
  for (i = 0; i < rows; i++) {
    for (j = 0; j < cols; j++) {
      fscanf(fileptr, "%d", &matrix[i][j]);
    }
  }
  matrixStruct *matrix_a = malloc(sizeof(matrixStruct *));
  matrix_a->rows = rows;
  matrix_a->cols = cols;
  matrix_a->pmatrix = matrix;
  return matrix_a;
}
/*
void checkMatrix(matrix *a, matrix *b)
{
}

void destroyMatrix(matrix *del)
{
}
*/
void printMatrix(matrixStruct *print)
{
  int i, j;
  
  for (i = 0; i < print->rows; i++) {
    for (j = 0; j < print->cols; j++) {
      printf("%d ", print->pmatrix[i][j]);
    }
    printf("\n");
  }
}
