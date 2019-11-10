#include "../include/error.h"

void Werror(char *s){
  if(DEBUG)
    printf("%s", s);
}

void Ferror(char *s){
  perror(s);
  exit(EXIT_FAILURE);
}
