
#define MAX_N 1000
#define MAX_M 20

#define MAX_STAR_IN_CONSTELL 20
#define MAX_STAR_IN_SKY 20000

#define STAR 1
#define PIVOT 9

struct Result {
  int y, x;
};

int sky_size = MAX_N;
int constellation_size = MAX_M;

int sky[MAX_N][MAX_N];

struct Position {
  int x, y;
};

enum Direction {
  NORMAL = 0,
  ROTATE_90 = 1,
  ROTATE_180 = 2,
  ROTATE_270 = 3
};

Position stars[MAX_STAR_IN_SKY];
int num_star_in_sky;
Position star_array[4][MAX_STAR_IN_CONSTELL];
int num_star_in_constell;

void init(int N, int M, int Map[MAX_N][MAX_N]) {
  sky_size = N;
  constellation_size = M;
  num_star_in_sky = 0;

  for (int y = 0; y < sky_size; ++y) {
    for (int x = 0; x < sky_size; ++x) {
      sky[y][x] = Map[y][x];
      if (sky[y][x] == STAR) {
        stars[num_star_in_sky].x = x;
        stars[num_star_in_sky].y = y;
        ++num_star_in_sky;
      }
    }
  }
}

void MakeRotationMap() {
  for (int k = 0; k < num_star_in_constell; ++k) {
    star_array[ROTATE_90][k].x = constellation_size - star_array[NORMAL][k].y - 1;
    star_array[ROTATE_90][k].y = star_array[NORMAL][k].x;

    star_array[ROTATE_180][k].x = constellation_size - star_array[NORMAL][k].x - 1;
    star_array[ROTATE_180][k].y = constellation_size - star_array[NORMAL][k].y - 1;

    star_array[ROTATE_270][k].x = star_array[NORMAL][k].y;
    star_array[ROTATE_270][k].y = constellation_size - star_array[NORMAL][k].x - 1;
  }

  for (int r = 0; r < 4; ++r) {
    int pivot_x = star_array[r][0].x;
    int pivot_y = star_array[r][0].y;
    for (int k = 1; k < num_star_in_constell; ++k) {
      star_array[r][k].x -= pivot_x;
      star_array[r][k].y -= pivot_y;
    }
  }
}

bool Match(int x, int y) {
  bool matching = false;
  for (int r = 0; r < 4; ++r) {
    if (star_array[r][0].x > x || (constellation_size - star_array[r][0].x) > (sky_size - x) ||
        star_array[r][0].y > y || (constellation_size - star_array[r][0].y) > (sky_size - y))
      continue;

    matching = true;
    for (int k = 1; k < num_star_in_constell; ++k) {
      if (sky[y + star_array[r][k].y][x + star_array[r][k].x] != STAR) {
        matching = false;
        break;
      }
    }
    if (matching == true) return true;
  }
  return matching;
}

Result MatchStars() {
  Result result;
  for (int k = 0; k < num_star_in_sky; ++k) {
    if (Match(stars[k].x, stars[k].y)) {
      result.x = stars[k].x;
      result.y = stars[k].y;
      return result;
    }
  }
  return result;
}

Result findConstellation(int stars[MAX_M][MAX_M])
{
  int count = 1;
  for (int y = 0; y < constellation_size; ++y) {
    for (int x = 0; x < constellation_size; ++x) {
      switch (stars[y][x]) {
      case STAR:
        star_array[NORMAL][count].x = x;
        star_array[NORMAL][count].y = y;
        ++count;
        break;

      case PIVOT:
        star_array[NORMAL][0].x = x;
        star_array[NORMAL][0].y = y;
        break;
      }
    }
  }
  num_star_in_constell = count;

  MakeRotationMap();

  return MatchStars();
}
