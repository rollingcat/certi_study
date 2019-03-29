/*
 * user_2-dim.cpp
 *
 *  Created on: Mar 28, 2019
 *      Author: hyowon
 */

int tmp[4][4];

void rotate_cw(int M, int object[4][4], int count){
	int (*src)[4] = object;
	int (*dst)[4] = tmp;

	int m = M-1;

	int opt_c = count % 4;
	int c = 0;

	while (true) {
		for (int j = 0; j < M; ++j) {
			for (int k = 0; k < M; ++k) {
				dst[k][m-j] = src[j][k];
			}
		}

		if (++c < opt_c) {
			int (*ptr)[4] = src;
			src = dst;
			dst = ptr;
		} else {
			break;
		}
	}

	if (dst == tmp) {
		for (int j = 0; j < M; ++j) {
			for (int k = 0; k < M; ++k) {
				object[j][k] = tmp[j][k];
			}
		}
	}
}

void rotate_ccw(int M, int object[4][4], int count){
	int (*src)[4] = object;
	int (*dst)[4] = tmp;

	int m = M-1;

	int opt_c = count % 4;
	int c = 0;

	while (true) {
		for (int j = 0; j < M; ++j) {
			for (int k = 0; k < M; ++k) {
				dst[m-k][j] = src[j][k];
			}
		}

		if (++c < opt_c) {
			int (*ptr)[4] = src;
			src = dst;
			dst = ptr;
		} else {
			break;
		}
	}

	if (dst == tmp) {
		for (int j = 0; j < M; ++j) {
			for (int k = 0; k < M; ++k) {
				object[j][k] = tmp[j][k];
			}
		}
	}
}





















