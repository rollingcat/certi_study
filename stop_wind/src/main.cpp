/*
바람아 멈추어다오

한 변의 길이가 N인 정사각형 모양의 땅이 있다. (한 변의 최대길이 : 100)


- 땅안의 모든 칸에는 깃발이 존재하는데 깃발은 세워져 있거나 쓰러져 있거나 둘 중 하나의 상태를 갖는다.

 init 함수를 통해 초기 땅 정보가 주어짐


- user code에서는 땅의 깃발 상태 정보를 아래 함수를 이용해서 확인할 수 있다.

 search(int y, int x, int size);

 (y,x)를 좌상단으로 하고 한 변의 길이가 size인 정사각형 크기의 땅 안에 세워져 있는 깃발의 수를 return 해준다.


- 이 땅에는 바람이 부는데, 바람이 불고 나면 몇몇 깃발들이 쓰러지게 된다.

 바람이 불고 나면 user code의 wind()함수가 호출되는데

 wind()함수는 방금 바람으로 쓰러진 깃발들의 정보(갯수와 위치)를 return 해주어야 한다.

 쓰러지는 깃발의 갯수는 1개이상 N(한변의 길이)보다 작거나 같다.


- 또한 땅주인이 쓰러진 깃발들을 세우기도 한다.

 쓰러진 깃발을 세우고 나면 user code의 setupflag()함수가 호출되는데

 setupflag()함수는 방금 땅주인이 세운 깃발들의 정보(갯수와 위치)를 return 해주어야 한다.

 세워지는 깃발의 갯수는 1개이상 N(한변의 길이)보다 작거나 같다.


- wind()함수와 setupflag()함수는 합쳐서 100번이하 호출된다.(한 번의 TC에)


- wind함수나 setupflag함수의 return값이 틀린 경우 오답처리되며

 search 함수의 호출 횟수가 적을 수록 좋다.


* main의 totalScore가 최대한 크도록 user code의 함수를 구현하시오.

*/


#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS    // Ignore build errors when using unsafe functions in Visual Studio.
#endif

#include <stdio.h>

#define MAX_N 100

struct COORDINATE {
	int x, y;
};

struct RESULT {
	int count;
	COORDINATE point[MAX_N];
};

enum STATE {
	CMD_SETUPFLAGS,
	CMD_WIND,
	CMD_END = 99
};

extern void init(int area[MAX_N][MAX_N], int N);
extern RESULT setupflag();
extern RESULT wind();

static int score;
static int searchCount;
static int N;
static int area[MAX_N][MAX_N];

int search(int top, int left, int size) {
	if (top < 0 || left < 0 || top >= N || left >= N || size <= 0) return 0;
	int sum = 0;
	for (register int i = top; i < top + size; ++i) {
		if (i >= N) break;
		for (register int j = left; j < left + size; ++j) {
			if (j >= N) break;
			if (area[i][j]) ++sum;
		}
	}
	++searchCount;
	return sum;
}

static void run() {
	int cmdCount;
	int correctCount = 0;
	int i, j, cmd;
	int orgArea[MAX_N][MAX_N];

	scanf("%d", &N);
	for (i = 0; i < N; ++i) {
		for (j = 0; j < N; ++j) {
			scanf("%d", &area[i][j]);
			orgArea[i][j] = area[i][j];
		}
	}
	score = 0;
	searchCount = 0;

	init(orgArea, N);

	cmdCount = 0;
	cmd = 0;
	while (1)	{
		int changedCount, x, y;
		STATE cmdType;
		RESULT result;
		int check[MAX_N][MAX_N] = { 0 };

		scanf("%d", &cmdType);
		if (cmdType == CMD_END) break;
		changedCount = 0;
		i = 0;

		while (1)	{
			int n;
			scanf("%d ", &n);
			if (n < 0) break;
			y = n / N;
			x = n % N;
			++changedCount;
			area[y][x] = cmdType == CMD_SETUPFLAGS ? 1 : 0;
			check[y][x] = 1;
			++i;
		}

		switch (cmdType) {
		case CMD_SETUPFLAGS:
			result = setupflag();
			break;
		case CMD_WIND:
			result = wind();
			break;
		}

		if (result.count == changedCount) {
			for (i = 0; i < changedCount; ++i) {
				if (check[result.point[i].y][result.point[i].x] != 1 ||
					area[result.point[i].y][result.point[i].x] == cmdType) {
					break;
				}
				check[result.point[i].y][result.point[i].x] = 0;
			}
			if (i == changedCount)
				++correctCount;
		}
		++cmd;
		++cmdCount;
	}
	int tcCutline;
	scanf("%d", &tcCutline);

	if ((correctCount == cmdCount) && (searchCount <= tcCutline)) {
			score = tcCutline - searchCount;
	}
	else {
		score = -1000000;
	}
}

int main() {
	int T, tc, totalScore = 0;

	setbuf(stdout, NULL);

	scanf("%d", &T);

	for (tc = 1; tc <= T; ++tc) 	{
		run();
		totalScore += score;
	}
	if (totalScore < 0) totalScore = 0;
	printf("#total score : %d\n", totalScore);
	if (totalScore > 0) {
		return 0; // success
	}
	return -1; // failure
}
