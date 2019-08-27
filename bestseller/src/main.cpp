#include <stdio.h>
#include <time.h>

#define MAX_NUM 10
#define MAX_ITEMS 20000

static int seed = 18043000;
static int pseudo_rand(void) {
	seed = seed * 214013 + 2531011;
	return (seed >> 16) & 0x7FFF;
}

static int numList[MAX_NUM];
extern void init();
extern void sell(int item_id, int num_sell);
extern void best_sellers(int num, int idList[MAX_NUM]);

int main() {
	time_t start = clock();

	freopen("sample_input.txt", "r", stdin);
	for (int tc = 0; tc <= 100; tc++)
		for (int t = 1; t <= 10; t++) {
			init();

			for (register int i = 0; i < 20000; i++) {
				int item_id = pseudo_rand() % MAX_ITEMS + 1;
				int num_sell = pseudo_rand();
				sell(item_id, num_sell);
			}

			best_sellers(t, numList);

			for (int i = 0; i < t; i++) {
				int ans = 0;
				scanf("%d", &ans);
				if (ans != numList[i]) {
					printf("FAIL\n");
					return 0;
				}

			}
		}
	printf("PASS\n");

	return 0;
}