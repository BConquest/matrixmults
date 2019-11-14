#include "../include/matrix.h"

int *allocateMatrix(int rows, int cols)
{
	int *ptr;

	ptr = calloc((rows*cols),sizeof(int));
	if (!ptr) Ferror("Cannot allocate space for matrix");
	
	return ptr;
}

int *getCol(matrix *a, int col)
{
	if (a->c <= col) Ferror("Invalid col");

	int r = 0;
	int *newCol = calloc(a->r,sizeof(int));
	
	if (!newCol) Ferror("Could not allocate space for new col");
	

	for (r = 0; r < a->r; r++) {
		newCol[r] = a->pmatrix[col+(r*(a->r+1))];
	}

	printf("\n");
	return newCol;
}

int *getRow(matrix *a, int row)
{
	if (a->r <= row) Ferror("Invalid row");
	
	int c = 0;
	int* newRow = calloc(a->c,sizeof(int));
	
	if (!newRow) Ferror("Could not allocate space for new row");
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
		Ferror("Error");
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
	if (a->c != b->r) Ferror("Matricies do not correlate");
	if (a->r > 50 || a->c > 50) Ferror("Matrix a is too large");
	if (b->r > 50 || b->c > 50) Ferror("Matrix b is too large");
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
