#include<stdio.h>

void quick_sort(int arr[], int left, int right);

struct Data {
	int paperId;
	int quotaNum;
	int profId;
};

Data data[1000] = { 0 };

Data* prof[1000 + 1][1000 + 1] = { 0 };
int interest_list[1000 + 1][1000 + 1] = { 0 };

int prof_num = 0;

int last_publish = 0;

void init(int N){
	printf("총교수 %d명\n", N);
	prof_num = N;
	for (int i = 0; i < prof_num + 1; ++i) {
		for (int k = 0; k < 1000 + 1; ++k) {
			prof[i][k] = nullptr;
			interest_list[i][k] = 0;
		}
	}
}

void interest(int mId1, int mId2, int timestamp){
	//printf("교수[%d] -> 교수[%d] 관심 (%d)\n", mId1, mId2, timestamp);
	++interest_list[mId1][0];
	interest_list[mId1][interest_list[mId1][0]] = mId2;
}

void publishThesis(int pId, int tId, int timestamp){
	printf("교수[%d] -> 논문[%d] 발표 (%d)\n", pId, tId, timestamp);
	int n = timestamp % 1000;

	for (int i = last_publish; i < n; ++i) {
		data[i].paperId = 0;
		data[i].profId = 0;
		data[i].quotaNum = 0;
	}

	data[n].paperId = tId;
	data[n].profId = pId;
	data[n].quotaNum = 0;
}

void quotation(int tId, int timestamp){
	printf("논문[%d] 인용 (%d)\n", tId, timestamp);
	int i = 1;
	for ( ; i < 1000 + 1; ++i) {
		if (data[i].paperId == tId)
			break;
	}
	++data[i].quotaNum;
}

void getTop(int pId, int timestamp, int tIdList[]){
	printf("교수[%d]의 관심 논문 10개는? (%d)\n", pId, timestamp);

	int arr[8] = { 20, 18, 50, 40, 9, 19, 5, 25 };
	quick_sort(arr, 0, 7);
	for (int i = 0; i < 8; ++i) {
		printf("%d ", arr[i]);
	}
}

void quick_sort(int arr[], int left, int right) {
	if (left >= right)
		return;

	int i, j, key, tmp;
	i = left;
	j = right + 1;
	key = arr[left];
	while (i < j) {
		do { ++i;
		} while (arr[i] < key);
		do { --j;
		} while (arr[j] > key);

		if (i < j) {
			tmp = arr[i];
			arr[i] = arr[j];
			arr[j] = tmp;
		}
	}

	tmp = arr[left];
	arr[left] = arr[j];
	arr[j] = tmp;

	quick_sort(arr, left, j - 1);
	quick_sort(arr, j + 1, right);
}
