
#define MAXR    99
#define MAXC    26

#define MIN_NUMBER -1000000000
#define MAX_NUMBER  1000000000

#define DASH '-'
#define nullstr '\0'
#define CHAR_0 48
#define CHAR_A 65

#define TO_NUMBER(n) (n - CHAR_0)
#define IS_CHAR(n) ((n == '-') || (n == ')') || (n == ',') || (n == '('))

#define MAX_STR 13

struct Addr {
	int r; int c;
};

struct Cell {
	char str[MAX_STR];
	long long value;
	bool calculated;
};

Cell table[MAXR][MAXC];
int size_row;
int size_col;

typedef long long (*FUNC)(Addr, Addr);

long long Calculate(int r, int c);

int BigCharToNum(char c) {
	return c - CHAR_A;
}

#define BigCharToNum(c) (c - CHAR_A)
#define NumCharToNum(c) (c - CHAR_0)
#define IsMark(c) (c < 48)
#define IsNumber(c) (c >= 45 && c <= 57)

long long ADD(Addr addr1, Addr addr2) {
	return Calculate(addr1.r, addr1.c) + Calculate(addr2.r, addr2.c);
}

long long SUB(Addr addr1, Addr addr2) {
	return Calculate(addr1.r, addr1.c) - Calculate(addr2.r, addr2.c);
}

long long MUL(Addr addr1, Addr addr2) {
	return Calculate(addr1.r, addr1.c) * Calculate(addr2.r, addr2.c);
}

long long DIV(Addr addr1, Addr addr2) {
	return Calculate(addr1.r, addr1.c) / Calculate(addr2.r, addr2.c);
}

long long MAX(Addr addr1, Addr addr2) {
	long long max_num = MIN_NUMBER;
	long long value = 0;
	int r = addr1.r;
	int c = addr1.c;
	if (addr1.r == addr2.r) {
		while (c <= addr2.c) {
			value = Calculate(r, c++);
			if (max_num < value) max_num = value;
		}
	} else {
		while (r <= addr2.r) {
			value = Calculate(r++, c);
			if (max_num < value) max_num = value;
		}
	}
	return max_num;
}

long long MIN(Addr addr1, Addr addr2) {
	long long min_num = MAX_NUMBER;
	long long value = 0;
	int r = addr1.r;
	int c = addr1.c;
	if (addr1.r == addr2.r) {
		while (c <= addr2.c) {
			value = Calculate(r, c++);
			if (min_num > value) min_num = value;
		}
	} else {
		while (r <= addr2.r) {
			value = Calculate(r++, c);
			if (min_num > value) min_num = value;
		}
	}
	return min_num;
}

long long SUM(Addr addr1, Addr addr2) {
	long long sum = 0;
	int r = addr1.r;
	int c = addr1.c;
	if (addr1.r == addr2.r) {
		while (c <= addr2.c)
			sum += Calculate(r, c++);
	} else {
		while (r <= addr2.r)
			sum += Calculate(r++, c);
	}
	return sum;
}

bool StringToValue(char* input, long long* value) {
	if (!IsNumber(input[0]))
		return false;

	bool minus = false;
	char* number = input;
	if (number[0] == DASH) {
		minus = true;
		number = &number[1];
	}

	int idx = 0;
	*value = NumCharToNum(number[idx++]);
	while (number[idx] != nullstr) {
		*value *= 10;
		*value += NumCharToNum(number[idx++]);
	}
	if (minus) *value *= -1;

	return true;
}

Addr StringToAddr(char* input) {
	Addr addr;
	addr.c = BigCharToNum(input[0]);
	addr.r = NumCharToNum(input[1]);
	if (IsMark(input[2])) {
		--addr.r;
		return addr;
	}
	addr.r *= 10;
	addr.r += NumCharToNum(input[2]);
	--addr.r;
	return addr;
}

void StringToFunc(char* input, FUNC* func, Addr* addr1, Addr* addr2) {
	switch (input[0]) {
	case 'A': *func = ADD; break;
	case 'D': *func = DIV; break;
	case 'S':
		if (input[2] == 'B') *func = SUB;
		else *func = SUM;
		break;

	case 'M':
		if (input[1] == 'U') *func = MUL;
		else if (input[1] == 'A') *func = MAX;
		else *func = MIN;
		break;
	}

	*addr1 = StringToAddr(&input[4]);
	if (IsMark(input[6]))
		*addr2 = StringToAddr(&input[7]);
	else
		*addr2 = StringToAddr(&input[8]);
}

void print_table() {
	/*
	for (int r = 0; r < size_row; ++r) {
		for (int c = 0; c < size_col; ++c) {
			if (table[r][c].calculated)
				printf("%12lld ", table[r][c].value);
			else {
				if (IsNumber(table[r][c].str[0]))
					printf("%11s! ", table[r][c].str);
				else 
					printf("%12s ", table[r][c].str);
			}
		}
		printf("\n");
	}
	printf("\n");
	*/
}

long long Calculate(int r, int c) {
	if (table[r][c].calculated)
		return table[r][c].value;

	if (table[r][c].str[0] == '\0') {
		table[r][c].calculated = true;
		table[r][c].value = 0;
		return 0;
	}

	long long value;
	if (!StringToValue(table[r][c].str, &value)) {
		Addr addr1, addr2;
		FUNC func = nullptr;
		StringToFunc(table[r][c].str, &func, &addr1, &addr2);
		value = func(addr1, addr2);

		table[r][c].calculated = true;
		table[r][c].value = value;
	}
	table[r][c].calculated = true;
	table[r][c].value = value;
	return value;
}

void mystrcpy(char* dest, const char* src) {
	int idx = 0;
	while (src[idx] != nullstr) {
		dest[idx] = src[idx];
		++idx;
	}
	dest[idx] = src[idx];
}

void init(int C, int R) {
	size_row = R;
	size_col = C;

	for (int r = 0; r < R; ++r) {
		for (int c = 0; c < C; ++c) {
			table[r][c].str[0] = nullstr;
			table[r][c].calculated = false;
		}
	}
}

void set(int col, int row, char input[]) {
	mystrcpy(table[row - 1][col - 1].str, input);
	table[row - 1][col - 1].calculated = false;
}

void update(int value[MAXR][MAXC]) {
	for (int r = 0; r < size_row; ++r) {
		for (int c = 0; c < size_col; ++c) {
			if (!IsNumber(table[r][c].str[0]))
				table[r][c].calculated = false;	// 함수는 무조건 다시 계산
		}
	}

	for (int r = 0; r < size_row; ++r) {
		for (int c = 0; c < size_col; ++c) {
			value[r][c] = (int)Calculate(r, c);
		}
	}
}
