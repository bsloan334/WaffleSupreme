#pragma once

#include <queue>
#include <cstring>

#include "Process.h"
#include "LongTerm.h"
#include "Dispatcher.h"
#include "CPU.h"
#include "Statistics.h"

using namespace std;

class ShortTerm {
public:
	ShortTerm(queue<Process*>* zeQ, LongTerm* sched, CPU* target, Statistics* stats_init) :
		zeQueue(zeQ), scheduler(sched), targetCPU(target), stats(stats_init) {}
	bool RunNextProcess();

private:
	LongTerm * scheduler;
	CPU* targetCPU;
	queue<Process*>* zeQueue;
	Statistics* stats;
	int cpuID;
	string outFileName = "JobOutputFile.txt";
};
