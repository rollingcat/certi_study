//============================================================================
// Name        : 2dim_rotate.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

//#include <iostream>
//using namespace std;
//
//int main() {
//	cout << "!!!Hello World!!!" << endl; // prints !!!Hello World!!!
//	return 0;
//}


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
				printf("object[%d][%d] = %d / %d\n", j, k, object[j][k], c);
			}
		}
		printf("\n");
	}

	return score;
}

int main() {
	setbuf(stdout, NULL);
	freopen("sample_input.txt", "r", stdin);

	printf("#total score : %d\n", test_rotate());

	return 0;
}
