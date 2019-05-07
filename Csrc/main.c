// This file corresponds to the TableauCounter.java file

#include <stdio.h>
#include <stdlib.h>

#include "tableau.c"

long int writeCount(Tableau* t){
  return 0;
}


int main(int argc, char *argv[]){
  
  char write = 0;
  if(argc > 1 && argv[1][0]=='-' && argv[1][1]=='w'){
    write = 1;
  }

  long int total;
  Tableau* tableau = getTableau();
  printTableau(tableau);

  if (write){
    total = writeCount(tableau);
  }else {
    // Do some OMP and MPI magic
  }

  printf("\nThere are %ld tableaux of this shape.\n", total);
  return 0;
}
