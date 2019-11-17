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

int *getCol(matrixStruct* matrix, int col)
{
  int i;

  int *returnCol = malloc(matrix->rows * sizeof(int));

  for (i = 0; i < matrix->rows; i++)
    returnCol[i] = matrix->pmatrix[i][col];

  return returnCol;
}

int *getRow(matrixStruct* matrix, int row)
{
  int i;

  int *returnRow = malloc(matrix->cols * sizeof(int));

  for (i = 0; i < matrix->cols; i++)
    returnRow[i] = matrix->pmatrix[row][i];

  return returnRow;
}

int **allocateMatrix(int rows, int cols)
{
  int i;

  int **arr = malloc(rows * sizeof(int *));

  if (!arr) {
    perror("Error Allocating Space for matrix");
    exit(1);
  }

  for (i = 0; i < rows; i++) {
    arr[i] = malloc(cols * sizeof(int));
    if (!arr[i]) {
      perror("Error Allocating Sub-Space for matrix");
      exit(1);
    }
  }

  return arr;
}

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
  matrixStruct *matrix_a = malloc(sizeof(matrixStruct));
  matrix_a->rows = rows;
  matrix_a->cols = cols;
  matrix_a->pmatrix = matrix;
  fclose(fileptr);
  return matrix_a;
}

void checkMatrix(matrixStruct *a, matrixStruct *b)
{
  if (a->cols != b->rows) {
    printf("ERROR: Matrix's cannot be multiplied together\n");
    exit(1);
  }
}

void destroyMatrix(matrixStruct *del)
{
  int i;

  for (i = 0; i < del->rows; i++) {
    free(del->pmatrix[i]);
  }
  free(del->pmatrix);
  free(del);
}

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

void writeToFile(char *filename, int **matrix, int rows, int cols)
{
  FILE *out = fopen(filename, "w");
  fprintf(out, "%d %d\n", rows, cols);
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      fprintf(out, "%d ", matrix[i][j]);
    }
  }
  fclose(out);
}
