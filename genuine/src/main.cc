/*
 * 유명한 판화 예술가가 있다.
그의 작품 고가로 거래되어 많은 모조품이 존재한다.
당신은 미술관으로부터 진품 감별을 의뢰받았다.

보안을 위해서 진품 전체를 받지 못하고 4등분한 그림 조각 4개와 임의의 위치 그림 조각 1개를 받았다.

(예)

그런데 보내준 미술관측 실수로 순서가 뒤섞여서 전달되었다.
이 그림들은 모두 디지털화(0,1)화 되어 있다.

(예)

당신은 우선 다섯 조각 이미지를 이용해서 원본 그림을 완성해야 한다.

그리고 검증해야 할 그림의 일부분이 전달된다.
이 그림 조각이 원본과 비교해서 95% 이상 일치한다면 진품(1),
아니면 모조품(0)으로 감별한다.

다음은 구현해야할 함수들이다.

void init(int size)
size: 원본 그림의 한 변 크기이다.
그림은 모두 정사각형이고 한 변의 크기는 16~256 이며 16의 배수이다.
매 test case마다 처음 1회 전달된다.

void sendImagePart(char imagePart[][MAX_IMAGE_SIZE])
원본 그림의 조각을 전달 받는다.
imagePart: 원본 그림의 조각이며 한 변의 크기가 init()에서 전달받은 size의 1/2이다.
init() 이후에 총 5회 연속 호출되며 모두 이미지 크기는 모두 같다.
원본을 4등분한 그림 조각 4개와 임의의 좌표를 좌상으로 하는 그림 조각 1개로 구성된다. (임의의 좌상 좌표는 y, x의 범위는 2 ~ (size / 2 - 2) 이다.)
그림 조각들은 순서 없이 전달되며 회전되지 않는다.

int testImage(char imagePart[][MAX_IMAGE_SIZE])
검사해야 하는 그림의 조각을 전달 받는다.
imagePart: 한 변의 크기가 init()에서 전달받은 size의 1/2인 그림 조각
test case마다 최대 50회 호출이 되며 좌 또는 우측으로 90도 회전된 그림일 수 있다. 또는 회전되지 않을 수 있다.
원본 그림과 비교해서 95% 이상 일치하면 1을 반환하고 아니면 0을 반환한다.
좌상 y,x 좌표가 8의 배수인 이미지가 전달된다.
예를 들어 size가 16이라면 전달되는 이미지의 좌상 좌표는 (0,0), (8,0), (0,8), (8,8) 중 하나가 된다.
*/

#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>

static unsigned int seed = 12345;
static unsigned int pseudo_rand(int max) {
  seed = ((unsigned long long)seed * 1103515245 + 12345) & 0x7FFFFFFF;
  return seed % max;
}

#define MAX_IMAGE_SIZE (256)

extern void init(int size);
extern void sendImagePart(char imagePart[][MAX_IMAGE_SIZE]);
extern int testImage(char imagePart[][MAX_IMAGE_SIZE]);

static int tc, N, M, halfN;
static char image[3][MAX_IMAGE_SIZE][MAX_IMAGE_SIZE];
static char imagePart[MAX_IMAGE_SIZE][MAX_IMAGE_SIZE];
static int PY[50], PX[50], psize, noise_ratio;

static void makeImage() {
  int gap = pseudo_rand(127) + 127;
  int cnt = 0;
  for (int i = 0; i < N; ++i) {
    int Ni = N - i - 1;
    for (int j = 0; j < N; ++j) {
      int rnd = pseudo_rand(257) + 256;
      if (pseudo_rand(rnd) >= rnd / 2)
        image[1][N - j - 1][i] = image[2][j][Ni] = image[0][i][j] = 1;
      else
        image[1][N - j - 1][i] = image[2][j][Ni] = image[0][i][j] = 0;
      ++cnt;
      if (cnt >= gap) {
        gap = pseudo_rand(127) + 127;
        cnt = 0;
      }
    }
  }
}

static void sendImage() {
  bool sended[5] = { false };
  int sendCnt = 0;
  while (sendCnt < 5) {
    int index = pseudo_rand(5);
    while (sended[index]) {
      if (index == 4) {
        index = 0;
        continue;
      }
      ++index;
    }
    sended[index] = true;
    printf("--- index: %d\n", index);
    int y, x;
    switch (index) {
    case 0: y = 0; x = 0; break;
    case 1: y = 0; x = halfN; break;
    case 2: y = halfN; x = 0; break;
    case 3: y = halfN; x = halfN; break;
    case 4:
      y = pseudo_rand(halfN - 4) + 2;
      x = pseudo_rand(halfN - 4) + 2;
    }
    for (int i = 0; i < halfN; ++i)
      for (int j = 0, yi = y + i; j < halfN; ++j)
        imagePart[i][j] = image[0][yi][x + j];
    sendImagePart(imagePart);
    ++sendCnt;
  }
}

static int used[128][128] = { 0 };
static int step = 0;
static void makeTestImage() {
  ++step;
  int r, y, x;
  r = pseudo_rand(3);
  y = pseudo_rand(halfN / 8 + 1) * 8;
  x = pseudo_rand(halfN / 8 + 1) * 8;
  for (int i = 0; i < halfN; ++i)
    for (int j = 0, yi = y + i; j < halfN; ++j)
      imagePart[i][j] = image[r][yi][x + j];
  int change = pseudo_rand(halfN * halfN * 0.12) + 1;
  while (change) {
    y = pseudo_rand(halfN * 3) / 3;
    x = pseudo_rand(halfN * 7) / 7;
    if (used[y][x] == step) {
      pseudo_rand(257);
      continue;
    }
    used[y][x] = step;
    imagePart[y][x] ^= 1;
    --change;
  }
}

static int run() {
  int score = 100;
  char correct[51];

  scanf("%d %d %d", &seed, &N, &M);
  halfN = N / 2;

  init(N);
  makeImage();
  sendImage();

  scanf("%s", correct);

  for (int m = 0; m < M; m++) {
    makeTestImage();
    char ret = testImage(imagePart) + '0';
    if (correct[m] != ret)
      score = 0;
  }

  printf("#%d : %d\n", tc, score);
  return score;
}

int main() {
  int T, totalScore = 0;

  setbuf(stdout, NULL);
  freopen("input.txt", "r", stdin);
  scanf("%d", &T);

  for (tc = 1; tc <= T; tc++)
    totalScore += run();

  printf("#total score : %d\n", totalScore / T);

  if (totalScore / T != 100) return -1;
  return 0;
}


