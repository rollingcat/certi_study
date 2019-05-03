/*
교수가 관심있어 하는 핫한 논문을 정리하고자 한다.


교수(id: 1~1,000)는 최소 2명이며 다른 교수들의 어떤 논문이 핫한지 관심(interest)있어 한다.

자신 포함 자신이 관심있어 하는 교수들의 모든 논문 중에서 최신(timestamp 1,000 이내) 논문들 중

인용 횟수에 따라 내림차순으로 정렬하여 최대 10개까지 보여줘야 한다.

만약 인용(quotation) 횟수가 같은 논문의 경우는 최신(timestamp가 클수록) 논문의 우선순위가 높다.

위 조건에 맞는(timestamp 1,000 이내인) 논문이 10편 미만인 경우

남은 논문들 중에서 부족한 수만큼 최신 논문을 포함한다. (인용 횟수 무관)

(역시 자신 포함 관심있어 하는 교수들의 논문이 대상이다)


timestamp는 1부터 최대 100,000까지 증가한다.

논문은 생성 순으로 id가 부여되며 1부터 최대 100,000까지 증가한다.


만약 다음과 같이 교수 2명만 있는 상태라고 가정할 경우

교수17: 논문1(1), 논문4, 논문5(3), 논문7(1)

교수25: 논문2(9), 논문3(3), 논문6(7)

괄호안 숫자는 인용된 수이다.

※아직 두 교수 사이에 interest관계는 없다.


교수17에 대한 핫한 논문을 검색하면 결과는 다음과 같다.

논문5(3), 논문7(1), 논문1(1), 논문4


교수25에 대한 핫한 논문을 검색하면 결과는 다음과 같다.

논문2(9), 논문6(7), 논문3(3)


이제 교수17이 교수25의 논문에 관심을 가지게 되었다면

교수17에 대한 핫한 논문 검색 결과는

논문2(9), 논문6(7), 논문5(3), 논문3(3), 논문7(1), 논문1(1), 논문4

가 된다. (논문 5가 논문3보다, 논문7이 논문1보다 최신논문이다.)


하지만 관심은 상호관계가 아니므로, 교수25에 대한 핫한 논문은 여전히

논문2(9), 논문6(7), 논문3(3)

이다.


물론 교수25가 교수17에 대한 관심을 가진다면

교수 25에 대한 핫한 논문 검색 결과는 교수17에 대한 핫한 논문 검색결과와 같이

논문2(9), 논문6(7), 논문5(3), 논문3(3), 논문7(1), 논문1(1), 논문4

이 될 것이다.


다음은 함수 설명이다.


void init(int N);
	N (2~1,000) : 총 교수 수.
	매 tc 초기에 1회 호출된다.


void interest(int pId1, int pId2, int timestamp);
	pId1, pId2 (1~1,000) : 교수 id. N을 초과하지 않는다.
	pId1 교수가 pId2 교수를 관심 대상으로 삼는다.
	최대 100,000회 호출 가능하다.


void publishThesis(int pId, int tId, int timestamp);
	pId (1~1,000) : 교수 id. N을 초과하지 않는다.
	tId (1~100,000) : 논문 id. 1부터 순차적으로 증가한다.
	pId 교수가 tId 논문을 게재한다.
	최대 100,000회 호출 가능하다.


void quotation(int tId, int timestamp);
	tId (1~100,000) : 논문 id. publishThesis()에 의해서 호출된 tId만 사용 가능하다.
	tId 논문이 인용된다.
	논문이 게재(publishThesis())될 당시 인용건수는 0이다.
	최대 100,000회 호출 가능하다.


void getTop(int pId, int timestamp, int tIdList[]);
	pId (1~1,000) : 교수 id. N을 초과하지 않는다.
	pId 교수가 관심있어 하는 논문들은 다음과 같은 우선순위로 내림차순 정렬해서 상위 최대 10개를 tIdList[] 저장 반환한다.
	1. 각 교수들의 논문의 timestamp가 주어진 timestamp와 1,000 이내인 경우 인용횟수(1), 최신(2) 순으로 정렬한다.
	2. 1에서 취합된 결과가 10개 미만인 경우 취합된 논문을 제외한 나머지 논문들 중 부족한 수만큼 최신논문을 내림차순 정렬 포함한다.
	최대 5,000회 호출 가능하다.
*/

#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#include<stdio.h>

extern void init(int N);
extern void interest(int pId1, int pId2, int timestamp);
extern void publishThesis(int pId, int tId, int timestamp);
extern void quotation(int tId, int timestamp);
extern void getTop(int tId, int timestamp, int tIdList[]);

static unsigned int seed = 12345;
static unsigned int pseudo_rand(int max) {
	seed = ((unsigned long long)seed * 1103515245 + 12345) & 0x7FFFFFFF;
	return seed % max;
}

#define MAX_PROFESSOR 1000
#define MAX_RATIO 10000
#define MAX_TIME 100000

static int answer_score;
static int professorN; // 2 ~ 1000
static int end_timestamp;
static int interestRatio; // 1 ~ 10000
static int publishRatio; // 1 ~ 10000
static int quotationRatio; // 1 ~ 10000
static int getTopRatio; // 1 ~ 500

static int productArrary[200000];
static int totalPCnt;
static int minPCnt;

static bool run(int t) {
	int pId1, pId2, tId, tIdList[10], ans_tIdList[10], rand_val;
	bool ret = true;
	bool interestStatus[MAX_PROFESSOR + 1][MAX_PROFESSOR + 1] = { false };

	scanf("%d%d%d%d%d%d%d", &seed, &professorN, &end_timestamp, &interestRatio, &publishRatio, &quotationRatio, &getTopRatio);
	init(professorN);

	for (int mId1 = 1; mId1 <= professorN; ++mId1) {
		interestStatus[mId1][mId1] = true;
		int m = professorN / 10 + 1;
		if (m > 10)
			m = 10;
		for (int i = 0; i < m; ++i) {
			do {
				pId2 = pseudo_rand(professorN) + 1;
			} while (interestStatus[mId1][pId2]);
			interest(mId1, pId2, 1);
			interestStatus[mId1][pId2] = true;
		}
	}
	minPCnt = totalPCnt = 1;

	for (int timestamp = 1; timestamp <= end_timestamp; ++timestamp) {
		rand_val = pseudo_rand(10000);
		if (rand_val < interestRatio) {
			pId1 = pseudo_rand(professorN) + 1;
			pId2 = pseudo_rand(professorN) + 1;
			int tryCnt = 5;
			do {
				pId2 = pseudo_rand(professorN) + 1;
				if (!interestStatus[pId1][pId2]) {
					interest(pId1, pId2, timestamp);
					interestStatus[pId1][pId2] = true;
					break;
				}
				--tryCnt;
			} while (tryCnt > 0);
		}

		rand_val = pseudo_rand(10000);
		if (rand_val < publishRatio) {
			pId1 = pseudo_rand(professorN) + 1;
			productArrary[totalPCnt] = timestamp;
			publishThesis(pId1, totalPCnt, timestamp);
			++totalPCnt;
		}

		rand_val = pseudo_rand(10000);
		if (rand_val < quotationRatio && totalPCnt - minPCnt > 0) {
			while (productArrary[minPCnt] < timestamp - 1000 && minPCnt < totalPCnt)
				minPCnt++;
			if (totalPCnt != minPCnt) {
				tId = pseudo_rand(totalPCnt - minPCnt) + minPCnt;
				quotation(tId, timestamp);
			}
		}

		rand_val = pseudo_rand(10000);
		if (rand_val < getTopRatio && totalPCnt > 0) {
			pId1 = pseudo_rand(professorN) + 1;
			for (int i = 0; i < 10; ++i) tIdList[i] = 0;
			getTop(pId1, timestamp, tIdList);

			for (int i = 0; i < 10; i++)
				scanf("%d", ans_tIdList + i);
			bool prt = true;
			for (int i = 0; i < 10; i++) {
				if (ans_tIdList[i] == 0) break;
				if (ans_tIdList[i] != tIdList[i]) ret = false;
			}
		}
	}

	return ret;
}

int main()
{
	int T;
	setbuf(stdout, NULL);
	freopen("sample_input_s.txt", "r", stdin);
	scanf("%d", &T);
	int totalScore = 0;
	for (int t = 1; t <= T; t++) {
		int score = 0;
		if (run(t)) score = 100;
		totalScore += score;
		printf("#%d : %d\n", t, score);
	}
	printf("#total score : %d\n", totalScore / T);
	if (totalScore / T != 100) return -1;
	return 0;
}
