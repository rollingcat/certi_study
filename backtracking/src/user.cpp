#include <stdio.h>

#define N 5

extern void swap(char* input, int i, int j);
extern void addString(char* input);

void printPermutation(int index, char* input) {
  if (index == N) {
    addString(input);
    //printf("%s\n", input);
    return;
  }

  for (int i = index; i < N; i++) {
	swap(input, index, i);
	printPermutation(index + 1, input);
	swap(input, index, i);
  }
}
