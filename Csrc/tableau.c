#include <stdlib.h>
#include <stdio.h>

typedef struct {
  int*     SHAPE;
  int*     WEIGHT;
  int***   SORTED_RULES;
  char     RECT;
  int      NUMLINES;
  int      WEIGHT_SUMMED;
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
  int i,j;
  Tableau* this = (Tableau*) malloc(sizeof(Tableau));
  this->SHAPE = (int*) malloc(numlines*sizeof(int));
  int length = 0;
  for(i=0; i < numlines; i++){
    this->SHAPE[i] = lengths[i];
    length += lengths[i];
  }
  this->NUMLINES = numlines;

  char rectangular = 1;
  for(i=1; i < numlines; i++){
    if(lengths[i] > lengths[i-1]){
      printf("Row lengths must be in non-increasing order. Please use English notation");
    }
    rectangular &= (lengths[i-1] == lengths[i]);
  }
  this->RECT = rectangular;
  this->WEIGHT = (int*) malloc(sizeof(int)*length);
  int iterator=0;
  for(i=0; i < numlines; i++){
    for(j=0; j < lengths[i]; j++){
      this->WEIGHT[iterator++] = weights[i][j];
    }
  }
  if(this->WEIGHT[0] != 1)
    printf("First weight must be 1");
  if(RECT && WEIGHT[WEIGHT.length - 1] != 1)
    print("Last weight of a rectangular tableau must be 1");

  this->WEIGHT_SUMMED = 0;
  for(i=0; i < length; i++){
    this->WEIGHT_SUMMED += this->WEIGHT[i];
  }

  int** rulesList = makeRules(this);
  int minindex = getListIndex(1, 1);
  int maxIndex = 0;
  if(RECT){
    for(i=0; i < length; i++){
      maxIndex += this->WEIGHT[i];
    }
    maxIndex = maxIndex - this->WEIGHT[length - 1];
  }else{
    maxIndex = 99;
  }

  for(i = 0; rulesList[i] != ((int*) NULL); i++);
  int rulesListSize = i;

  for(i=0; i < rulesListSize; i++){
    if(rulesList[i][0] <= minIndex ||
       rulesList[i][1] <= minIndex ||
       rulesList[i][0] >= maxIndex ||
       rulesList[i][1] >= maxIndex){
      rulesList[i][0] = -5;
      rulesList[i][1] = -5;
    }
  }

  // Sort rulesList and remove bad values.
  for (i = 0; i < rulesListSize; i++) {
    for (j = i + 1; j < n; j++){
      if (i == 0 && rulesList[j][0] == -5 && rulesList[j][0] == -5){
	rulesList[j][0] = rulesList[rulesListSize - 1][0];
	rulesList[j][1] = rulesList[rulesListSize - 1][1];
	rulesListSize--;
	j--;
      }
      if (rulesList[i][0] > rulesList[j][0]) {
	int temp0 =  rulesList[i][0];
	int temp1 =  rulesList[i][1];
	rulesList[i][0] = rulesList[j][0];
	rulesList[i][1] = rulesList[j][1];
        rulesList[j][0] = temp0;
        rulesList[j][1] = temp1;
      }
    }
  }

  for (i = 0; i < rulesListSize; i++) {
    rulesList[i][0]--;
    rulesList[i][1]--;
  }

  this->SORTED_RULES = sortRules(this, rulesList, rulesListSize, weights);
  
  free(lengths);
  return this;

}

private int getCellIndex(Tableau* t, int row, int col) {
  int g = 0;
  int i,j;
  for(i = 0; i < row; i++) {
    for(j = 0; j < t->SHAPE[i]; j++) {
      if(i + 1 == row && j + 1 == col)
	break;
      g++;
    }
  }
  return g;
}

//Indices start at 1 because that's fun
private int getListIndex(Tableau* t, int row, int col) {
  int g = 0;
  int h = 0;
  int i,j;
  for(i = 0; i < row; i++) {
    for(j = 0; j < t->SHAPE[i]; j++) {
      if(i + 1 == row && j == col)
	break;
      h += t->WEIGHT[g++];
    }
  }
  return --h;
}

int** makeRules(Tableau* t){

  int current = 0;
  int** rulesList = (int**) malloc(sizeof(int)*(1 << 15));
  int i,j,k;

  for (i=1; i < t->NUMLINES; i++){
    for(j=1; j<=t->SHAPE[i]; j++){
      rulesList[current] = (int*)malloc(2*sizeof(int));
      rulesList[current][0] = getListIndex(i, j);
      rulesList[current][1] = getListIndex(i + 1, j) - t->WEIGHT[getCellIndex(i + 1, j)] + 1;
      current++;
    }
  }

  for (i=1; i <= t->NUMLINES; i++){
    for(j=1; j< t->SHAPE[i-1]; j++){
      rulesList[current] = (int*)malloc(2*sizeof(int));
      rulesList[current][0] = getListIndex(i, j);
      rulesList[current][1] = getListIndex(i, j) + 1;
      current++;
    }
  }

  for (i=1; i <= t->NUMLINES; i++){
    for(j=1; j <= t->SHAPE[i-1]; j++){
      for(k=1; k < t-WEIGHT[getCellIndex(i,j)]; k++) {
	rulesList[current] = (int*)malloc(2*sizeof(int));
	rulesList[current][0] = getListIndex(i, j) - k;
	rulesList[current][1] = getListIndex(i, j) - k+ 1;
	current++;
      }
    }
  }

  return rulesList; 

}

int getN(Tableau* t){
 
  if(t->RECT){
    return (t->WEIGHT_SUMMED - 2);
  } else {
    return (t->WEIGHT_SUMMED - 1);
  }
}

int*** sortRules(Tableau* t, int** rules, int rulesLen, int** weights){

  int*** sortedRules;
  int counts[getN(t)];
  int c;
  int i,j;

  for(i=0; i < getN(t); i++){
    c= 1;
    for (j=0; j< rulesLen; j++){
      if(rules[j][0] == i || rules[j][1] == i){
	c++;
      }
    }
    counts[i] = c;
  }
  
  sortedRules = (int***) malloc(sizeof(int**)*getN(t));
  int* maxima = flatten(getCellMaxima(t, weights));

}


int** getCellMaxima(Tableau* t, int** weights){

  int** maxima = (int**) malloc(sizeof(int*)*t->NUMLINES);
  int i,j;

  for(i=0; i < t->NUMLINES; i++){
    int sum = 0;
    for(j=0; j < t->SHAPE[i]; j++){
      sum += weights[i][j];
    }
    if (i==0){
      maxima[i] = (int*) malloc(sizeof(int)*(sum-1));
    } else {
      if ( i == t->NUMLINES - 1 && t->RECT){
	maxima[i] = (int*) malloc(sizeof(int)*(sum - weights[t->NUMLINES - 1][t-SHAPE[t->NUMLINES] - 1]));
      } else {
	maxima[i] = (int*) malloc(sizeof(int)*sum);
      }
    }
  }

  int h;
  

}
