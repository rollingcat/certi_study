
#include <stdio.h>

#define MAX_IMAGE_SIZE (256)
#define MAX_IMAGE_SIZE_HALF 128
#define THRESHOLD 5
static char image_parts[5][MAX_IMAGE_SIZE_HALF][MAX_IMAGE_SIZE_HALF];

enum QUADRANT {
  LT_0,
  RT_1,
  LB_2,
  RB_3,
  NONE
};

int n = 0;
int half_n = 0;
int sending_count = 0;
int threshold = 0;

QUADRANT parts_order[5] = { NONE };

int abs(int k) {
  if (k < 0)
    return k * -1;
  return k;
}

QUADRANT mirror(QUADRANT in) {
  switch (in) {
  case LT_0:
    return RB_3;
  case RT_1:
    return LB_2;
  case LB_2:
    return RT_1;
  case RB_3:
    return LT_0;
  }
}

QUADRANT getQuadrant(int x, int y) {
  int mid = half_n / 2;
  if (x < mid && y < mid)
    return RB_3;
  if (x >= mid && y < mid)
    return LB_2;
  if (x < mid && y >= mid)
    return RT_1;
  return LT_0;
}

void intersect(int a_l, int a_t, int a_w, int a_h,
               int s_r, int s_b,
               int* out_x, int* out_y, int* out_w, int* out_h) {
  int s_l = 0;
  int s_t = 0;

  int a_r = a_l + a_w;
  int a_b = a_t + a_h;

  *out_x = s_l > a_l ? s_l : a_l;
  int out_r = s_r < a_r ? s_r : a_r;

  *out_y = s_t > a_t ? s_t : a_t;
  int out_b = s_b < a_b ? s_b : a_b;

  *out_w = out_r - *out_x;
  *out_h = out_b - *out_y;

//
//  if (s_x > a_x) {
//    *out_x = s_x;
//    *out_w = a_x + a_w;
//  } else {
//    *out_x = a_x;
//    *out_w = s_w - a_x;
//  }
//
//  if (s_y > a_y) {
//    *out_y = s_y;
//    *out_h = a_y + a_w;
//  } else {
//    *out_y = a_y;
//    *out_h = s_h - a_y;
//  }
}

void init(int size){
  n = size;
  half_n = n / 2;
  sending_count = 0;
  for (int i = 0; i < 5; ++i)
    parts_order[i] = NONE;
  threshold = (half_n * half_n) * THRESHOLD;
}

//bool doCompare(int x_size, int y_size, char** a, char** b, int threshold = 1);
//bool doCompare(int x_size, int y_size, char** a, char** b, int threshold) {
//  int error = 0;
//  for (int y = 0; y < y_size; ++y) {
//    for (int x = 0; x < x_size; ++x) {
//      if (a[y][x] != b[y][x]) {
//        ++error;
//        if (threshold == error)
//          return false;
//      }
//    }
//  }
//}

bool doCompare(char a[][MAX_IMAGE_SIZE_HALF], char b[][MAX_IMAGE_SIZE_HALF],
    int a_x, int a_y, int b_x, int b_y, int w, int h) {
  printf("a: %d %d %d %d / b: %d %d %d %d\n", a_x, a_y, w, h, b_x, b_y, w, h);
  for (int y = 0; y < h; ++y) {
    for (int x = 0; x < w; ++x) {
      if (a[a_y + y][a_x + x] != b[b_y + y][b_x + x])
        return false;
    }
  }
  return true;
}

//int A[4][3][2] = {
//    {{1, 11}, {2, 12}, {3, 13}},
//    {{4, 14}, {5, 15}, {6, 16}},
//    {{7, 17}, {8, 18}, {9, 19}},
//    {{10, 20}, {11, 21}, {12, 22}}};
//int (*tmp)[3][2] = A;
//int (*tmp2)[2] = A[2];
//
//printf("--------------%d\n", tmp2[2][1]);

QUADRANT IsCross(int a, int b) {
  int x_pivot = -n + 2;
  int y_pivot = -n + 2;
  int area_x, area_y, area_w, area_h;

  char (*a_ptr)[MAX_IMAGE_SIZE_HALF] = image_parts[a];
  char (*b_ptr)[MAX_IMAGE_SIZE_HALF] = image_parts[b];

  int count = n * 2 - 4;
  for (int y = 0; y < count; ++y) {
    x_pivot = -n + 2;
    y_pivot = -n + 2 + y;
    for (int x = 0; x < count; ++x) {
      intersect(x_pivot, y_pivot, half_n, half_n,
          half_n - 1, half_n - 1,
          &area_x, &area_y, &area_w, &area_h);
      printf("pivot(%d, %d) intersect - %d %d %d %d\n", x_pivot, y_pivot,
          area_x, area_y, area_w, area_h);

      if (doCompare(a_ptr, b_ptr,
          abs(area_x - x_pivot), abs(area_y - y_pivot),
          area_x, area_y, area_w, area_h)){
        printf("intersect - %d %d %d %d\n", area_x, area_y, area_w, area_h);
        return getQuadrant(area_x, area_y);
      }

//      char** a_start = image_parts[a][abs(area_y - y_pivot)][abs(area_x - x_pivot)];
//      char** b_start = image_parts[b][area_x][area_y];
//      if (doCompare(area_w, area_h, a_start, b_start)) {
//        return getQuadrant(area_x, area_y);
//      }
      ++x_pivot;
    }
  }
  return NONE;
}

void makeCompleteImage() {
  int center = -1;
  QUADRANT result = IsCross(3, 4);
  printf("IsCross(3, 4) --> %d\n", result);
  if (result != NONE) {
    parts_order[3] = result;
    result = IsCross(2, 4);
    printf("IsCross(2, 4) --> %d\n", result);
    if (result != NONE) { // center is 4
      parts_order[2] = result;
      center = 4;
    } else { // center is 3
      parts_order[4] = mirror(parts_order[3]);
      parts_order[3] = NONE;
      center = 3;
    }
  } else {
    result = IsCross(1, 2);
    printf("IsCross(1, 2) --> %d\n", result);
    if (result != NONE) {
      parts_order[1] = result;
      result = IsCross(0, 2);
      printf("IsCross(0, 2) --> %d\n", result);
      if (result != NONE) { // center is 2
        parts_order[0] = result;
        center = 2;
      } else { // center is 1
        parts_order[2] = mirror(parts_order[1]);
        parts_order[1] = NONE;
        center = 1;
      }
    } else
      center = 0;
  }

  for (int k = 0; k < 5; ++k) {
    if (parts_order[k] == NONE && center != k)
      parts_order[k] = IsCross(k, center);
    printf("--- order: %d\n", parts_order[k]);
  }
}

void sendImagePart(char imagePart[][MAX_IMAGE_SIZE]){

  for (int y = 0; y < half_n; ++y) {
    for (int x = 0; x < half_n; ++x) {
      image_parts[sending_count][y][x] = imagePart[y][x];
      printf("%d ", image_parts[sending_count][y][x]);
    }
    printf("\n");
  }
  printf("sendImagePart %d ------------------------\n", sending_count);

  ++sending_count;
  if (sending_count == 5)
    makeCompleteImage();
}

int testImage(char imagePart[][MAX_IMAGE_SIZE]){
  //
  return 0;
}



























