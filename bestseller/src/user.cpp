
#define MAX_NUM 10
#define MAX_ITEMS 20000

long long selling[MAX_ITEMS];

struct Heap {
  int data[MAX_NUM + 2];
  int cnt;

  void swap(int a, int b) {
    if (a == b) return;
    int c = data[a];
    data[a] = data[b];
    data[b] = c;
  }

  void push(int i) {
    int idx = ++cnt;
    data[idx] = i;
    while (idx > 1) {
      if (selling[data[idx]] < selling[data[idx >> 1]]) {
        swap(idx, idx >> 1);
        idx = idx >> 1;
      } else break;
    }
  }

  void pop() {
    swap(cnt--, 1);
    int idx = 1;
    int next = 1;

    while (true) {
      if (idx << 1 <= cnt && selling[data[idx << 1]] < selling[data[next]]) {
        next = idx << 1;
      }
      if ((idx << 1) + 1 <= cnt && selling[data[(idx << 1) + 1]] < selling[data[next]]) {
        next = (idx << 1) + 1;
      }

      if (idx == next) break;

      swap(idx, next);
      idx = next;
    }
  }
};

Heap heap;

void init() {
  heap.cnt = 0;
  for (int k = 0; k < MAX_ITEMS; ++k)
    selling[k] = 0;
}

void sell(int item_id, int num_sell) {
  selling[item_id - 1] += num_sell;
}

void best_sellers(int num, int idList[MAX_NUM]) {
  int n = num;
  while (n > 0) {
    heap.push(--n);
  }

  for (int k = num; k < MAX_ITEMS; ++k) {
    if (selling[k] < selling[heap.data[1]]) continue;

    heap.pop();
    heap.push(k);
  }

  for (int k = num - 1; k >= 0; --k) {
    idList[k] = heap.data[1] + 1;
    heap.pop();
  }

  for (int k = 1; k < num; ++k) {
    if (selling[idList[k - 1] - 1] == selling[idList[k] - 1]) {
      if (idList[k - 1] < idList[k]) {
        int c = idList[k];
        idList[k] = idList[k - 1];
        idList[k - 1] = c;
      }
    }
  }
}
