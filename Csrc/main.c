// This file corresponds to the TableauCounter.java file

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#include "tableau.c"
#include "parallelCounter.c"

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
    int THREAD_COUNT = (tableau->SORTED_RULES[0][0][0] - 1) * (tableau->SORTED_RULES[1][0][0] - 1);
    ParallelCounter** counters = (ParallelCounter**) malloc(sizeof(ParallelCounter*)*THREAD_COUNT);
    printf("There were %d threads started\n", THREAD_COUNT);
    int i;
    #pragma omp parallel for
    for(i = 0; i < THREAD_COUNT; i++){
      counters[i] = makeParallelCounter(tableau, i, THREAD_COUNT);
      run(counters[i]);
    }

    total = 0;
    for(i=0; i < THREAD_COUNT; i++){
      total += counters[i]->count;
    }
  }

  printf("\nThere are %ld tableaux of this shape.\n", total);
  return 0;
}
