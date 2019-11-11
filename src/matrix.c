#include "../include/matrix.h"

int *allocateMatrix(int rows, int cols)
{
  int *ptr;
  int i;

  ptr = malloc((rows*cols)*sizeof(int));

  if (!ptr) {
    Ferror("Cannot allocate space for matrix");
  }

  for (i = 0; i < rows*cols; i++) {
    ptr[i] = 0;
  }
  
  return ptr;
}

int *getRow(matrix *a, int row)
{
  int c = 0;
  int* newRow = malloc(a->c*sizeof(int));

  if (!newRow) {
    Ferror("Could not allocate space");
  }

  for (c = 0; c < a->c; c++) {
    newRow[c] = a->pmatrix[(row*a->c)+c];
  }
  return newRow;
}

matrix *initMatrix(char *filename)
{
  FILE *fileptr;
  int temp_rows, temp_cols, i;
  int *temp_matrix;

  fileptr = fopen(filename, "r");

  if (!fileptr) {
    printf("Error opening <%s>", filename);
    Ferror("Error");
  }

  fscanf(fileptr, "%d", &temp_rows);
  fscanf(fileptr, "%d", &temp_cols);
  if (DEBUG)
    printf("matrix dimensions r: %d c: %d\n", temp_rows, temp_cols);

  temp_matrix = allocateMatrix(temp_rows, temp_cols);
  
  for (i = 0; i < temp_rows*temp_cols; i++)
    fscanf(fileptr, "%d", &temp_matrix[i]);

  if (DEBUG)
    printMatrix(temp_matrix, temp_rows, temp_cols);

  matrix *matrix1 = malloc(sizeof(matrix));
  matrix1->r = temp_rows;
  matrix1->c = temp_cols;
  matrix1->pmatrix = temp_matrix;

  fclose(fileptr);
  return matrix1;
}

void checkMatrix(matrix *a, matrix *b)
{
  if (a->c != b->r) {
    Ferror("Matricies do not correlate");
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
  int i, j, c = 0;

  for (i = 0; i < rows; i++) {
    for (j = 0; j < cols; j++) {
      printf("%d ", ptr[c]);
      c++;
    }
    printf("\n");
  }
}
