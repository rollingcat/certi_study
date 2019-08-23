
#include <stdio.h>

#define MAX_N 12
#define MAX_CORE 12
#define MAX_LENGTH (MAX_N*MAX_N)

struct Core {
  int x;
  int y;
};

Core cores[MAX_CORE];
int num_core;

int max_connection;
int min_length;

int map[MAX_N][MAX_N];
int map_size;

const int dx[4] = { 0, 1, 0, -1 };
const int dy[4] = { -1, 0, 1, 0 };


void init() {
  int N;
  scanf("%d", &N);
  map_size = N;
  num_core = 0;
  max_connection = 0;
  min_length = MAX_LENGTH;

  for (int y = 0; y < N; ++y) {
    for (int x = 0; x < N; ++x) {
      scanf("%d", &map[y][x]);

      if (map[y][x] == 1) {
        if (x != 0 && x != (map_size -1) && y != 0 && y != (map_size -1)) {
          cores[num_core].x = x;
          cores[num_core].y = y;
          ++num_core;
        }
      }
    }
  }
}

void calculate(int core_count, int connection, int length) {
//  printf("calculate(%d, %d, %d)\n", core_count, connection, length);
//  for (int y = 0; y < map_size; ++y) {
//    for (int x = 0; x < map_size; ++x) {
//      printf("%d ", map[y][x]);
//    }
//    printf("\n");
//  }
  if (core_count == num_core) {
    if (connection > max_connection) {
      max_connection = connection;
      min_length = length;
    } else if (connection == max_connection && length < min_length) {
      min_length = length;
    }
    return;
  }

  if (max_connection > connection + (num_core - core_count))
    return;

  for (int d = 0; d < 4; ++d) {
    int pos_x = cores[core_count].x;
    int pos_y = cores[core_count].y;
    bool connection_success = false;
    int cable = 0;
    while (1) {
      pos_x += dx[d];
      pos_y += dy[d];

      if (pos_x < 0 || pos_x == map_size || pos_y < 0 || pos_y == map_size) {
        connection_success = true;
        break; // connected!
      }

      if (map[pos_y][pos_x] == 1) break; // cannot connect

      map[pos_y][pos_x] = 1;  // add cable
      ++cable;
    }
    if (connection_success == true) {
      calculate(core_count + 1, connection + 1, length + cable);
    }

    while (cable != 0) {  // remove cable
      pos_x -= dx[d];
      pos_y -= dy[d];
      map[pos_y][pos_x] = 0;
      --cable;
    }
  }

  calculate(core_count + 1, connection, length);
}

int main(void) {
  int test_case;
  int T;
  freopen("sample_input.txt", "r", stdin);
  setbuf(stdout, NULL);
  scanf("%d", &T);

  for (test_case = 1; test_case <= T; ++test_case)
  {
    init();
    calculate(0, 0, 0);
    printf("#%d %d\n", test_case, min_length);
  }
  return 0;
}
