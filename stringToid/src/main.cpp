#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

bool strcmp(const char* str1, const char* str2){
  int i = 0;
  while (!((str1[i] == 0)&(str2[i] == 0))){
    if (str1[i] != str2[i]) return false;
    i++;
  }
  return true;
}

extern int convertStr2Int(char str[7]);
extern void convertInt2Str(int value, char str[7]);

int main(){
  freopen("input.txt", "r", stdin);
  //freopen("output.txt", "w", stdout);

  int TC = 0, score = 0;
  char str[7], answer[7];

  scanf("%d", &TC);

  for (int tc = 0; tc < TC; tc++){
    scanf("%s", str);
    int data = convertStr2Int(str);
    convertInt2Str(data, answer);
    if (strcmp(str, answer) == true) {
      score++;
    }
    else {
      printf("[Wrong Answer] right : %s, mine : %s\n", str, answer);
    }
  }
  if (score == TC) {
    printf("total score : %d\n", score);
  }
}
