#ifndef __error_h__
#define __error_h__

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

/*
 * Print a warning for non-fatal errors.
 * Should be called with Werror(NULL) if no prompt is given.
 */
void Werror(char *);
/*
 * Print error message from errno and exit with an exit failure.
 * Should be called with Ferror(NULL) if no prompt is given.
 */
void Ferror(char *);

#endif
