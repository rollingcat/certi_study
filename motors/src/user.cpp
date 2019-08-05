
#include <stdio.h>

#define MAX_GROUP 100
#define MAX_ENGINEER 50

struct Engineer {
	int group_id_;
	int assemble_time_;

	int remain_time_;
	Engineer* next_;
};

struct Group {
	int num_;
	bool is_last_;

	int busy_;
	int pending_work_;
	Engineer* waiting_;

	Engineer* getFastestEngineer();
	void assignEngineer();	
	void doneWork(Engineer*);
};

Engineer engineers[MAX_GROUP][MAX_ENGINEER];

Group groups[MAX_GROUP];
int total_group = 0;

Engineer* working = 0;
int complete = 0;

int timer = 0;

void doQueueing(Engineer** queue, Engineer* assignee) {
	Engineer* ptr = *queue;
	if (ptr == 0) {
		*queue = assignee;
		return;
	}
	Engineer* prev = 0;
	while (ptr != 0) {
		if (assignee->remain_time_ > ptr->remain_time_) {
			prev = ptr;
			ptr = ptr->next_;
		}
		else {
			if (prev == 0) {
				assignee->next_ = ptr;
				return;
			}
			prev->next_ = assignee;
			assignee->next_ = ptr;
			return;
		}
	}
	prev->next_ = assignee;
}

void Group::assignEngineer() {
	while (waiting_ != 0 && pending_work_ > 0) {
		Engineer* assignee = getFastestEngineer();
		--pending_work_;

		doQueueing(&working, assignee);
		++busy_;
	}
}

Engineer* Group::getFastestEngineer() {
	Engineer* assignee = waiting_;
	waiting_ = waiting_->next_;
	assignee->next_ = 0;
	return assignee;
}

void Group::doneWork(Engineer* engineer) {
	engineer->remain_time_ = engineer->assemble_time_;
	doQueueing(&waiting_, engineer);	

	--busy_;
	if (is_last_)
		++complete;

	if (pending_work_ != 0)
		assignEngineer();
}

void quickSort(Engineer* arr, int left, int right) {
	if (left >= right)
		return;

	int i, j;
	int key;
	Engineer tmp;
	i = left;
	j = right + 1;
	key = arr[left].remain_time_;
	while (i < j) {
		do {
			++i;
		} while (arr[i].remain_time_ < key);
		do {
			--j;
		} while (arr[j].remain_time_ > key);

		if (i < j) {
			tmp = arr[i];
			arr[i] = arr[j];
			arr[j] = tmp;
		}
	}

	tmp = arr[left];
	arr[left] = arr[j];
	arr[j] = tmp;

	quickSort(arr, left, j - 1);
	quickSort(arr, j + 1, right);
}


void print() {
	printf("TOTAL GROUP: %d\n", total_group);
	for (int i = 0; i < total_group; ++i) {
		printf("Group[%d](%d) : ", i, groups[i].num_);
		for (int j = 0; j < groups[i].num_; ++j) {
			printf("%d ", engineers[i][j].assemble_time_);
		}
		printf("\n");
	}
}

void printWaiting() {
	printf("TOTAL GROUP: %d\n", total_group);
	for (int g = 0; g < total_group; ++g) {
		printf("Group[%d](%d) : ", g, groups[g].num_);
		Engineer* ptr = groups[g].waiting_;
		while (ptr != 0) {
			printf("%d ", ptr->remain_time_);
			ptr = ptr->next_;
		}
		printf("\n");
	}
}

void printWorking() {
	Engineer* ptr = working;
	while (ptr != 0) {
		printf("[%d| %d/%d] - ", ptr->group_id_, ptr->remain_time_, ptr->assemble_time_);
		ptr = ptr->next_;
	}
	printf("\n");
}

void proceedWorking() {
	Engineer* done = working;
	int time = done->remain_time_;

	if (time > 0) {
		Engineer* ptr = working;
		while (ptr != 0) {
			ptr->remain_time_ -= time;
			ptr = ptr->next_;
		}
	}
	working = working->next_;
	done->next_ = 0;

	Group* group = &groups[done->group_id_];
	group->doneWork(done);

	if (group->is_last_ == false) {
		++group;
		group->pending_work_ += 1;
		group->assignEngineer();
	}
	return;
	printWorking();
	printf("-------------------\n");
	for (int g = 0; g < total_group; ++g) {
		printf("{%d| %d | %d | %d} ", g, groups[g].num_, groups[g].busy_, groups[g].pending_work_);
	}
    	printf("\n");
}

void proceed(int time) {
	int proceed_time = time;
	int proceeding = 0;
	Engineer* ptr = working;
	while (ptr != 0) {
		proceeding = ptr->remain_time_;
		if (proceeding <= proceed_time) {
			proceedWorking();
			proceed_time -= proceeding;
			ptr = working;
		}
		else
			break;
	}
	if (proceed_time > 0) {
		ptr = working;
		while (ptr != 0) {
			ptr->remain_time_ -= proceed_time;
			ptr = ptr->next_;
		}
	}
	timer += time;
}

void init(int N, int E, int divisionNo[5000], int assemblyTime[5000]) {
	int i = 0;
	total_group = N;
	for (i = 0; i < total_group; ++i) {
		groups[i].num_ = 0;
		groups[i].is_last_ = false;
		groups[i].busy_ = 0;
		groups[i].pending_work_ = 0;
		groups[i].waiting_ = 0;		
	}
	groups[total_group - 1].is_last_ = true;

	for (i = 0; i < E; ++i) {
		int g = divisionNo[i];
		Engineer* engineer = &engineers[g][groups[g].num_];
		engineer->group_id_ = g;
		engineer->assemble_time_ = engineer->remain_time_ = assemblyTime[i];
		engineer->next_ = 0;
		++groups[g].num_;
	}

	print();

	for (int g = 0; g < total_group; ++g) {
		quickSort(engineers[g], 0, groups[g].num_ - 1);
		Engineer* ptr = groups[g].waiting_ = &engineers[g][0];
		for (int i = 0; i < (groups[g].num_ - 1); ++i) {
			ptr->next_ = &engineers[g][i + 1];
			ptr = ptr->next_;
		}
	}

	printWaiting();
}

void addOrder(int time, int number) {
	printf("time[%d] - order: %d\n", time, number);
	proceed(time - timer);

	groups[0].pending_work_ += number;
	groups[0].assignEngineer();
	printWorking();
	printf("========================================");
	for (int g = 0; g < total_group; ++g) {
		printf("{%d| %d | %d | %d} ", g, groups[g].num_, groups[g].busy_, groups[g].pending_work_);
	}
	printf("\n");
}

int interimCheck(int time, int _working[MAX_GROUP]) {
	proceed(time - timer);

	for (int g = 0; g < total_group; ++g) {
		_working[g] = groups[g].busy_ + groups[g].pending_work_;
		printf("[%d] %d ", g, _working[g]);
	}
	printf("\n complete: %d\n", complete);
	return complete;
}
