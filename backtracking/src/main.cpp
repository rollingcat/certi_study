
#include <stdio.h>
#define N 5

char str[5 * 4 * 3 * 2 * 1][N + 1];
int cntStr;

void init() {
  cntStr = 0;
}

void _strcpy(char* dest, const char* src) {
  char *cp = dest;
  while (*src != '\0') {
    *(cp++) = *(src++);
  }
  *(cp) = '\0';
}

void _qsort(char* input, int l, int r) {
  if (l >= r) return;
  int i = l;
  int j = r;
  int pivot = input[(i + j) / 2];
  while (i <= j) {
    while (input[i] < pivot) i++;
    while (input[j] > pivot) j--;
    if (i <= j) {
      char t = input[i];
      input[i] = input[j];
      input[j] = t;
      i++;
      j--;
    }
  }

  if (l<j) _qsort(input, l, j);
  if (i<r) _qsort(input, i, r);
}

void addString(char* input) {
  _strcpy(str[cntStr], input);
  cntStr++;
}

bool isSame(const char* a, const char *b) {
  while (*a != '\0') {
    if (*a++ != *b++) return false;
  }
  return true;
}

bool check(const char* input) {
  if (cntStr != 5 * 4 * 3 * 2 * 1) return false;

  char tmp[N + 1];
  _strcpy(tmp, input);
  _qsort(tmp, 0, N - 1);

  for (int i = 1; i < cntStr; i++) {
    _qsort(str[i], 0, N - 1);
    if (isSame(tmp, str[i]) == false) return false;
  }
  return true;
}

void swap(char* input, int i, int j) {
  char t = input[i];
  input[i] = input[j];
  input[j] = t;
}

void printPermutation(int index, char* input);

int main(void) {
  freopen("input.txt", "r", stdin);
  int T = 0;
  int score = 0;
  scanf("%d", &T);

  for (int t = 0; t<T; t++) {
    init();
    char input[N + 1] = { '\0', };
    scanf("%s", input);
    printPermutation(0, input);
    if (check(input) == true) score++;
  }

  printf("#total score : %d / %d\n", score, T);
  if (score == T) return 0;
  else return -1;
}
