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

int main(int argc, char *argv[])
{
	if(DEBUG) {
		printf("Debug mode is on.\n");
	}
  Werror("sah");
  Ferror(NULL);
	return 0;
}
