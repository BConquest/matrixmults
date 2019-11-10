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


int *allocateMatrix(int, int);
void printMatrix(int *, int, int);

int main(int argc, char *argv[])
{
  FILE *file1, *file2;
	if(DEBUG) {
		printf("Debug mode is on.\n");
	}
  
  file1 = fopen(argv[1], "r");
  file2 = fopen(argv[1], "r");

  if (!file1){
    Ferror("Error opening file 1");
  } else if (!file2) {
    Ferror("Error opening file 2");
  }

  return 0;
}

void printMatrix(int *ptr, int inner, int outer)
{
  int i, j;
  
  for (i = 0; i < inner; i++)
  {
    for (j = 0; j < outer; j++)
    {
      printf("%d ", ptr[i+j]);
    }
    printf("\n");
  }

}

int *allocateMatrix(int inner, int outer)
{
  int* ptr;
  int i;

  ptr = malloc((inner*outer)*sizeof(int));
  
  if(!ptr) {
    Ferror("Cannot allocate bomb");
  }

  for (i = 0; i < inner*outer; i++) {
    ptr[i] = i;
  }

  return ptr;
}
