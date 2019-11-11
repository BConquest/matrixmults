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
#include "./include/matrix.h"

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

  /*int *temp = getRow(matrix1, 0);
  int *temp2 = getCol(matrix1, 0);

  for(int i = 0; i < matrix1->r; i++) {
    printf("%d\n", temp2[i]);
  }
  for(int i = 0; i < matrix1->c; i++) {
    printf("%d ", temp[i]);
  }
  printf("\n");
  
  free(temp);
  free(temp2);*/
  destroyMatrix(matrix1);
  destroyMatrix(matrix2);
  return 0;
}
