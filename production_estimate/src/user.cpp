
#define MAX_N    100
#define MAX_TOOL 50

struct Tool {
  int process_id;

  int proc_time;
  int remain_time;

  int getTime() {
    return (remain_time == 0) ? proc_time : remain_time;
  }
};

struct Queue {
  Tool* tools[MAX_TOOL];
  int cnt;

  void swap(Tool* a, Tool* b) {
    Tool* c = a;
    a = b;
    b = c;
  }

  void push(Tool* tool) {
  }

  void pop() {

  }
};

Queue working;
int complete;

struct Process {
  Tool tools[MAX_TOOL];
  int tool_num;

  Queue idle;

  int lot_num;
  bool last;

  void add_tool(int procTime) {
    tools[tool_num].proc_time = procTime;
    tools[tool_num].remain_time = 0;

    idle.push(&tools[tool_num]);
  }

  void assign() {
    while (idle.cnt > 0 && lot_num > 0) {
      working.push(idle.tools[1]);
      idle.pop();
      --lot_num;
    }
  }
};

Process process[MAX_N];
int process_num;

#define is_last(n) (n == (process_num - 1))

int current_time;

void finish() {
  while (working.cnt > 0 && working.tools[1]->remain_time == 0) {
    Tool* done = working.tools[1];
    process[done->process_id].idle.push(done);
    working.pop();

    if (is_last(done->process_id)) complete++;
    else process[done->process_id + 1].lot_num++;
  }
}

void run(int time) {
  for (int p = 0; p < process_num; ++p) {
    for (int t = 0; t < process[p].tool_num; ++t) {
      process[p].tools[t].remain_time -= time;
    }
  }
  current_time += time;

  finish();

  for (int p = 0; p < process_num; ++p) {
    process[p].assign();
  }
}

void init(int N) {
  process_num = N;
}

void setupTool(int T, int stepNo[5000], int procTime[5000]) {
  for (int t = 0; t < T; ++t) {
    process[stepNo[t]].add_tool(procTime[t]);
  }
}

void addLot(int time, int number) {
  int time_diff = time - current_time;
  while (!time_diff) {
    if (working.tools[1]->remain_time <= time_diff) {
      run(working.tools[1]->remain_time);
    } else {
      run(time_diff);
    }
    time_diff = time - current_time;
  }

  process[0].lot_num += number;
  process[0].assign();
}

int simulate(int time, int wip[MAX_N]) {
  return 0;
}
