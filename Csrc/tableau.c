#include <stdlib.h>
#include <stdio.h>

typedef struct {
  int*     SHAPE;
  int*     WEIGHT;
  int*** SORTED_RULES;
  char      RECT;
} Tableau;

void printTableau(Tableau* t){
  return;
}
Tableau* makeTableau(int** weights, int* lengths, int numlines);

Tableau* getTableau(){
  FILE* tableauIn = fopen("../Tableau.txt", "r");
  int numlines = 0;
  int* lengths = malloc(sizeof(int)*(1<<7));
  size_t len;
  char* line = NULL;
  int read;

  while ((read = getline(&line, &len, tableauIn)) != -1) {
    lengths[numlines]=read;
    numlines++;
  }

  int** weights = (int**) malloc(sizeof(int*)*numlines);
  int i, j;
  for(j = 0; j < numlines; j++) {
    weights[j] = (int*) malloc(sizeof(int)*lengths[j]);
  }

  fclose(tableauIn);
  tableauIn = fopen("../Tableau.txt", "r");

  for(i = 0; i < numlines; i++) {
    for(j = 0; j < lengths[i]; j++) {
      if(!fscanf(tableauIn, "%d", &weights[i][j])) {break;}
    }
  }

  fclose(tableauIn);

  return makeTableau (weights, lengths, numlines);

}

Tableau* makeTableau(int** weights, int* lengths, int numlines){




  return NULL;

}
