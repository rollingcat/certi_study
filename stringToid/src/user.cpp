
#include <stdio.h>

#define char_a 97

#define BIT_MASK_OP 0

#if BIT_MASK_OP

#define char_mask 31

void printBinary(int num) {
  int mask;
  for (int i = 31; i >= 0; --i) {
    mask = 1 << i;
    printf("%d", (num & mask) ? 1 : 0);
    if (!(i % 5)) printf(" ");
  }
  printf("\n");
}

int convertStr2Int(char str[7]){
  int v = 0;
  for (int c = 0; c < 6; ++c) {
    v <<= 5;
    v += (str[c] - char_a);
//    printBinary(v);
  }
  return v;
}

void convertInt2Str(int value, char str[7]) {
  int v = value;
  for (int i = 5; i >= 0; --i) {
    str[i] = (v & char_mask) + char_a;
    v >>= 5;
  }
//  printf("%s\n", str);
}
#else
int convertStr2Int(char str[7]){
  int v = 0;
  int pow = 1;
  for (int c = 0; c < 6; ++c) {
    v += (str[c] - char_a) * pow;
    pow *= 26;
  }
  return v;
}

void convertInt2Str(int value, char str[7]) {
  int v = value;
  for (int i = 0; i < 6; ++i) {
    str[i] = (v % 26) + char_a;
    v /= 26;
  }
}
#endif
