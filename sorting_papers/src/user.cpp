#include<stdio.h>

struct Data {
	int paperId;
	int quotaNum;
	int profId;
	int time;
};

Data data[1000] = { 0 };

//Data* prof[1000 + 1][1000 + 1] = { 0 };

bool interset_status[1000 + 1][1000 + 1] = { false };

int prof_num = 0;

int last_publish = 0;

int paper_num = 0;

Data* sorting[1000] = { 0 };

void quick_sort(int arr[], int left, int right);
void quick_sort_quotation(Data* arr[], int left, int right);

void init(int N){
//	printf("init(%d)\n", N);
	prof_num = N;
	for (int i = 0; i < prof_num + 1; ++i) {
		for (int k = 0; k < prof_num + 1; ++k) {
			interset_status[i][k] = false;
		}
	}
}

void interest(int mId1, int mId2, int timestamp){
	//printf("Prof[%d] ---interest--> Prof[%d] (%d)\n", mId1, mId2, timestamp);
	interset_status[mId1][mId2] = true;
}

void publishThesis(int pId, int tId, int timestamp){
//	printf("Prof[%d] ---publish---> Paper[%d] (%d)\n", pId, tId, timestamp);
	int n = timestamp % 1000;

	if (n <= last_publish) {
		for (int i = last_publish + 1; i < 1000 + 1; ++i) {
			data[i].paperId = 0;
			data[i].profId = 0;
			data[i].quotaNum = 0;
			data[i].time = 0;
		}
		last_publish = 0;
	}

	for (int i = last_publish + 1; i < n; ++i) {
		data[i].paperId = 0;
		data[i].profId = 0;
		data[i].quotaNum = 0;
		data[i].time = 0;
	}

	data[n].paperId = tId;
	data[n].profId = pId;
	data[n].quotaNum = 0;
	data[n].time = timestamp;

	last_publish = n;

	++paper_num;
}

void quotation(int tId, int timestamp){
//	printf("Paper[%d] Quota (%d)\n", tId, timestamp);
	int i = 1;
	for ( ; i < 1000 + 1; ++i) {
		if (data[i].paperId == tId) {
			data[i].quotaNum++;
			break;
		}
	}
}

void getTop(int pId, int timestamp, int tIdList[]){
	printf("***** getTop(Prof[%d]) (%d)\n", pId, timestamp);


	int count = 0;
	for (int i = 1; i < 1000 + 1; ++i) {
		if (data[i].paperId == 0)
			continue;
		if (interset_status[pId][data[i].profId]) {
			sorting[count] = &data[i];
			++count;
		}
	}

	quick_sort_quotation(sorting, 0, count - 1);

	for (int i = 0; i < 10; ++i) {
		tIdList[i] = sorting[i]->paperId;
		printf("%d ", sorting[i]->paperId);
	}
	printf("\n");

//	int arr[8] = { 20, 18, 50, 40, 9, 19, 5, 25 };
//	quick_sort(arr, 0, 7);
//	for (int i = 0; i < 8; ++i) {
//		printf("%d ", arr[i]);
//	}
}

void quick_sort_quotation(Data* arr[], int left, int right) {
	if (left >= right)
		return;

	int i, j, key, key_time;
	Data* tmp;
	i = left;
	j = right + 1;
	key = arr[left]->quotaNum;
	key_time = arr[left]->time;
	while (i < j) {
		do { ++i;
			if (i == right) break;
		} while (arr[i]->quotaNum > key || (arr[i]->quotaNum == key && arr[i]->time > key_time));
		do { --j;
		} while (arr[j]->quotaNum < key || (arr[j]->quotaNum == key && arr[j]->time < key_time));

		if (i < j) {
			tmp = arr[i];
			arr[i] = arr[j];
			arr[j] = tmp;
		}
	}

	tmp = arr[left];
	arr[left] = arr[j];
	arr[j] = tmp;

	quick_sort_quotation(arr, left, j - 1);
	quick_sort_quotation(arr, j + 1, right);
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
