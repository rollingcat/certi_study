/*
2-Dim 회전을 연습한다.

시계방향(0: clockwise) 회전과 반시계방향(1: counterclockwise)을 연습한다.

구현해야 하는 함수는 다음과 같다.

extern void rotate_cw(int M, int object[4][4], int count);

extern void rotate_ccw(int M, int object[4][4], int count);

MxM(최대4) 크기의 object가 주어지고 rotate_cw는 시계방향으로 rotate_ccw는 반시계방향으로 90도 회전시킨다.

count는 90도 회전을 몇번 시킬지를 나타낸다.
*/

#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>

static int object[4][4];

extern void rotate_cw(int M, int object[4][4], int count);
extern void rotate_ccw(int M, int object[4][4], int count);

static int test_rotate() {
	int N;
	int score = 100;
	scanf("%d", &N);
	for (int i = 0; i < N; ++i) {
		int M;
		// load object
		scanf("%d", &M);
		for (int j = 0; j < M; ++j) {
			for (int k = 0; k < M; ++k) {
				scanf("%d", &object[j][k]);
			}
		}
		int c, count;
		scanf("%d %d", &c, &count);
		if (c == 0) { // 0: clockwise
			rotate_cw(M, object, count);
		}
		else { // 1: counterclockwish
			rotate_ccw(M, object, count);
		}
		// result
		for (int j = 0; j < M; ++j) {
			for (int k = 0; k < M; ++k) {
				scanf("%d", &c);
				if (object[j][k] != c) score = 0;
			}
		}
	}

	return score;
}

int main() {
	setbuf(stdout, NULL);
	freopen("sample_input.txt", "r", stdin);

	printf("#total score : %d\n", test_rotate());

	return 0;
}
