#include <stdio.h>
#include <stdlib.h>

typedef struct{
  Tableau* tableau;
  int*** RULES;
  int N;
  int R;
  int T;
  long count;
} ParallelCounter;

char valid(ParallelCounter* p, int* entries, int w){
  Tableau* t = p->tableau;
  if (entries[w] > p->RULES[w][0][0]){
    return 0;
  }
  int i;
  for (i = 1; i < t->SORTED_RULES_SHAPE[w];i++){
    if(!(entries[p->RULES[w][i][0]] < 0 || entries[p->RULES[w][i][1]] < 0) &&
       entries[p->RULES[w][i][0]] > entries[p->RULES[w][i][1]]){
      return 0;
    }
  }
  return 1;
}

char absValid(ParallelCounter* p, int* entries){
  int i;
  for(i=0; i < p->N; i++){
    if (! valid(p, entries, i)){
      return 0;
    }
  }
  return 1;
}


ParallelCounter* makeParallelCounter(Tableau* t, int range, int totalThreads){
  ParallelCounter* p = (ParallelCounter*) malloc(sizeof(ParallelCounter));
  p->tableau = t;
  p->RULES = t->SORTED_RULES;
  p->N = getN(t);
  p->R = range;
  p->T = totalThreads;
  p->count = 0;

  return p;
}

void run(ParallelCounter* p){
  int w = p->N - 1; // w, for working index, ranges from 0 to N-1
  int r = 2; // r is row
  int c = 2; // c is col
  int i,j;

  for(i=0; i < p->R; i++){
    c++;
    if (c > p->T / (p->RULES[0][0][0] - 1) + 1) {
      r++;
      c = 2;
    }
  }
  if (r == c) {return;}
  int* entries = (int*) malloc(sizeof(int)*p->N);
  // We dont know how big this should be. We just want to allocate enough memory.
  int* options = (int*) malloc(sizeof(int)*p->N); 
  int current = 0;
  
  entries[0] = r;
  entries[1] = c;
  for(i = 2; i < p->N; i++){
    entries[i] = -1;
  }
  if(!valid(p, entries,1)){
    return;
  }
  
  for(i = 2; i < p->N; i++){
    entries[i] = i + (i >= r ? 1 : 0);
    if(entries[i] >= c)
      entries[i]++;
    if(entries[i] == r)
      entries[i]++;
  }

  if(absValid(p, entries)){
    p->count++;
  } else {
    w = 2;
    for(; w < p->N && valid(p, entries, w);w++); //No body to for loop
  }

  for(; entries[0] == r && entries[1] == c && w >= 0;){
    //1. Clear everything below w
    for(i = w+1; i < p->N; i++){
      if(entries[i] > 0){
	options[current] = entries[i];
	current++;
      }
      entries[i] = -1;
    }

    //2. Sort options
    for (i = 0; i < current; i++) {
      for (j = i + 1; j < current; j++){
	if (options[i] > options[j]) {
	  int temp =  options[i];
	  options[i] = options[j];
	  options[j] = temp;
	}
      }
    }

    //3. If we cannot increases e[w], decrease w
    if(current == 0 || options[current - 1] < entries[w]){
      w--;
    } else {
      //4. increase e[w]

      for(i = 0; i < current; i++){
	if(entries[w] < options[i]){
	  break;
	}
      }
      if(entries[w] > 0){
	options[current] = entries[w];
	current++;
      }
      entries[w] = options[i];
      for(; i < current - 1; i++){
	options[i] = options[i+1];
      }
      options[current - 1] = -99;
      current--;

      //5. Validate.  Increment entries[w] if possible
      if(valid(p, entries, w)){
	//6. Fill the table.  Happens in step 3
	if ( w < p->N - 1){
	  w++;
	}
	//7. Check for valid Tableau
	if(entries[p->N - 1] > 0 && valid(p, entries, w)){
	  p->count++;
	}
      }
    }

  }

  printf("thread %d found %ld tableau(x)", p->R, p->count);
}
