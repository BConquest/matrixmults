#include "../include/matrix.h"

int *allocateMatrix(int rows, int cols)
{
	int *ptr;

	ptr = calloc((rows*cols),sizeof(int));
	if (!ptr) {
		perror("Not enough space for matrix allocation\n");
		exit(1);
	}
	return ptr;
}

int *getCol(matrix *a, int col)
{
	if (a->c <= col) {
		printf("Request of invalid column %d\n", col);
		exit(1);
	}

	int r = 0;
	int *newCol = calloc(a->r,sizeof(int));

	if (!newCol) {
		perror("Not enough space for column allocation\n");
		exit(1);
	}

	for (r = 0; r < a->r; r++) newCol[r] = a->pmatrix[col+(r*(a->r+1))];

	return newCol;
}

int *getRow(matrix *a, int row)
{
	if (a->r <= row) {
		printf("Request of invalid row %d\n", row);
		exit(1);
	}

	int c = 0;
	int* newRow = calloc(a->c,sizeof(int));

	if (!newRow) {
		perror("Not enough space for row allocation\n");
		exit(1);
	}
	for (c = 0; c < a->c; c++) newRow[c] = a->pmatrix[(row*a->c)+c];

	return newRow;
}

matrix *initMatrix(char *filename)
{
	FILE *fileptr;
	int temp_rows = 0, temp_cols = 0, i;
	int *temp_matrix;

	fileptr = fopen(filename, "r");

	if (!fileptr) {
		printf("Error opening <%s>", filename);
		perror("File Error");
		exit(1);
	}

	fscanf(fileptr, "%d", &temp_rows);
	fscanf(fileptr, "%d", &temp_cols);
	if (DEBUG) printf("matrix dimensions r: %d c: %d\n"
			, temp_rows, temp_cols);

	temp_matrix = allocateMatrix(temp_rows, temp_cols);

	for (i = 0; i < temp_rows*temp_cols; i++)
		fscanf(fileptr, "%d", &temp_matrix[i]);
	if (i != temp_rows*temp_cols && DEBUG)
		printf("Input matrix did not contain enough values initiliazed"
				"to 0\n");

	if (DEBUG) printMatrix(temp_matrix, temp_rows, temp_cols);

	matrix *a = malloc(sizeof(matrix));
	a->r = temp_rows;
	a->c = temp_cols;
	a->pmatrix = temp_matrix;

	fclose(fileptr);
	return a;
}

void checkMatrix(matrix *a, matrix *b)
{
	if (a->c != b->r) {
		printf("Matricies do not correlate\n");
		exit(1);
	}
	if (a->r > 50 || a->c > 50) {
		printf("Matrix a is too large\n");
		exit(1);
	}
	if (b->r > 50 || b->c > 50) {
		printf("Matrix b is too large\n");
		exit(1);
	}
}

void destroyMatrix(matrix *del)
{
	free(del->pmatrix);
	free(del);
}

void printMatrix(int *ptr, int rows, int cols)
{
	int i = 0, j = 0, c = 0;

	for (i = 0; i < rows; i++) {
		for (j = 0; j < cols; j++) printf("%d ", ptr[c++]);
		printf("\n");
	}
}
