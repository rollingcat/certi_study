/*
우면 모터스는 분임조를 통해서 자동차를 생산하고 있다.
자동차 생산은 여러 분임조가 순차적으로 부품을 조립을 하여 이루어진다.
첫 번째(0) 분임조에 조립을 끝내면 다음 분임조(1)가 이어받아서 조립하는 식이다.
이렇게 순차적으로 처리해서 N-1번째 분임조가 조립을 마치면 자동차가 완성된다.
(N : 3~100, 분임조간 전달 시간은 무시된다)

각 분임조에는 같은 일을 하는 엔지니어들이 여러명(1~50) 있는데, 엔지니어 한명이 1대의 자동차 조립을 담당한다.
엔지니어마다 조립을 수행하는데 걸리는 시간이 다르다. (조립 시간 : 5 ~ 2,000)

분임조에 도착한 차량은 해당 분임조의 쉬고 있는 엔지니어들 중 조립시간이 가장 짧은 엔지니어에게 할당되어 조립이 시작된다.
분임조에 대기중인 차량이 있을 때 쉬고 있는 엔지니어가 있어서는 안된다.
각 엔지니어는 조립 도중에 멈출 수 없고 다른 엔지니어에게 작업을 넘길 수 없다.
반드시 할당 받은 엔지니어가 작업을 끝내고 다음 분임조에게 전달해야 한다.

분임조의 모든 엔지니어가 작업을 하고 있을 경우, 해당 분임조에 전달된 조립중 차량은 대기 중 상태가 된다.
만약 대기중인 차량이 존재하고 동시에 일을 끝낸 엔지니어가 여러 명이라면 조립시간이 짧은 엔지니어가 작업 우선권이 있다.
모든 분임조를 통과해 완성된 자동차는 물류 창고에 저장된다.


// 함수 설명

void init(int N, int E, int divisionNo[5000], int assemblyTime[5000])
N : 분임조 수
E : 엔지니어 수
divisionNo : 엔지니어가 속한 분임조
assemblyTime : 엔지니어의 조립 시간


void addOrder(int time, int number)
time : 주문이 들어온 시각
number : 주문량


int interimCheck(int time, int working[MAX_N])
time : 중간점검 시각
working[] : 분임조별 작업량 (작업량은 분임조에서 조립중인 자동차 수와 조립 대기중인 자동차 수를 더한 값이다)

time 시각에 완성된 자동차 수량을 반환한다.


첫번째 TC 일부를 예로 들면
init() 함수를 통해서 3개 분임조의 51명 엔지니어 정보가 들어온다.

 0 ( 24) :  5 10 26 24 22  6 21 16  6 39 10 37 11 20 21 19 28  9  7 10 10  9 35 32
 1 ( 13) : 11 31  6 30 28 36 13 28 17 36 30 34 37
 2 ( 14) : 16  8 29 12 22 36 37 21 22 26 22  6 11 23


첫번째 addOrder() 를 통해서 0 time에 47대 주문량이 들어온다.

division  :  0  1  2
engineer  : 24 13 14
busy      : 24  0  0
wait      : 23  0  0


시간이 지남에 따라 addOrder() 를 통해서 계속 주문이 들어온다.
이후 interimCheck() 를 통해서 42 time 에 중간점검을 실시하면 다음과 같이 된다.

division  :  0  1  2
engineer  : 24 13 14
busy      : 13 13  9
wait      :  0 42  0
completion:  9
반환되는 배열값은 13, 55, 9 이며 완성된 자동차 수는 9이다.
*/

#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <stdio.h>

#define MAX_N    100
#define MAX_ENGINEER 50
#define MAX_TIME 100000
#define MIN_ASSEMBLY_TIME 5
#define MAX_ASSEMBLY_TIME 6000

extern void init(int N, int E, int divisionNo[5000], int assemblyTime[5000]);
extern void addOrder(int time, int number);
extern int interimCheck(int time, int working[MAX_N]);

static unsigned int seed = 54321;
static unsigned int pseudo_rand(int max) {
  seed = ((unsigned long long)seed * 1103515245 + 12345) & 0x7FFFFFFF;
  return seed % max;
}

static int testCase;

static int run() {
  int divisionN, division[MAX_N * MAX_ENGINEER];
  int enginnerN, assemblyTime[MAX_N * MAX_ENGINEER];
  int time, number, endTime;
  int retCarN, correctN;
  int score = 100;
  int retDivision[MAX_N];
  int correctDivision[MAX_N];
  int enginnerCntStep[MAX_N] = { 0 };

  scanf("%d %d %d %d", &divisionN, &enginnerN, &endTime, &seed);

  for (int e = 0; e < enginnerN; ++e) {
    division[e] = pseudo_rand(divisionN);
    while (enginnerCntStep[division[e]] >= MAX_ENGINEER)
      division[e] = (division[e] + 1) % divisionN;
    assemblyTime[e] = pseudo_rand(endTime / divisionN) + MIN_ASSEMBLY_TIME;
    if (assemblyTime[e] > MAX_ASSEMBLY_TIME)
      assemblyTime[e] = MAX_ASSEMBLY_TIME;
    ++enginnerCntStep[division[e]];
  }
  for (int d = 0; d < divisionN; ++d) {
    if (enginnerCntStep[d] == 0) {
      division[enginnerN] = d;
      assemblyTime[enginnerN] = pseudo_rand(endTime / divisionN / 2) + MIN_ASSEMBLY_TIME;
      if (assemblyTime[enginnerN] > MAX_ASSEMBLY_TIME)
        assemblyTime[enginnerN] = MAX_ASSEMBLY_TIME;
      ++enginnerN;
      ++enginnerCntStep[d];
    }
  }
  init(divisionN, enginnerN, division, assemblyTime);
  time = 0;
  addOrder(time, pseudo_rand(enginnerCntStep[0] * 2) + 1);

  while (1) {
    int cmd;
    time += pseudo_rand(endTime / enginnerN) + 10;
    if (time > endTime) {
      time = endTime;
      cmd = 100;
    }
    else
      cmd = pseudo_rand(100);
    if (cmd < 70) {
      number = pseudo_rand(enginnerCntStep[0]) + 1;
      addOrder(time, number);
    }
    else {
      retCarN = interimCheck(time, retDivision);
      if (divisionN < 12) {
        for (int d = 0; d < divisionN; d++)
          scanf("%d", &correctDivision[d]);
        scanf("%d", &correctN);
        if (retCarN != correctN)
          score = 0;
        for (int d = 0; d < divisionN; d++) {
          if (retDivision[d] != correctDivision[d])
            score = 0;
        }
      }
      else {
        for (int d = 0; d < divisionN; ++d) {
          if (score > 0) score -= retDivision[d];
          else score += retDivision[d];
          score += retCarN * 2;
        }
      }
    }
    if (cmd == 100) break;
  }
  if (divisionN >= 12) {
    int sum;
    scanf("%d", &sum);
    if (sum == score)
      return 100;
    return 0;
  }
  return score;
}

int main() {
  setbuf(stdout, NULL);
  int totalScore = 0;
  int T;

  //freopen("input.txt", "r", stdin);
  scanf("%d", &T);

  for (testCase = 1; testCase <= T; ++testCase) {
    int score = run();
    printf("#%d : %d\n", testCase, score);
    totalScore += score;
  }
  printf("#total score : %d\n", totalScore / T);

  if (totalScore / T != 100) return -1;
  return 0;
}


