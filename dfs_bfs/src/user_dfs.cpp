
int king[100][5] = { {0} };
int current_king = 0;

void resetKings() {
	for (int i = 0; i < 100; ++i) {
		for (int j = 0; j < 5; ++j) {
			king[i][j] = 0;
		}
	}
}

int searchKing(int t) {
	for (int son = 0; son < 5; ++son) {
		if (king[t][son] != 0) {
			if (king[t][son] > current_king) {
				return king[t][son];
			} else {
				return searchKing(king[t][son]);
			}
		} else {
			return 0;
		}
	}
}


void dfs_init(int N, int path[100][2]){
	resetKings();

	for (int i = 0; i < N; ++i) {
		for (int k = 0; k < 5; ++k) {
			if (king[path[i][0]][k] == 0) {
				king[path[i][0]][k] = path[i][1];
				break;
			}
		}
	}
}

int dfs(int n) {
	current_king = n;

	int new_king = searchKing(n);

	if (new_king == 0)
		return -1;
	return new_king;
}
