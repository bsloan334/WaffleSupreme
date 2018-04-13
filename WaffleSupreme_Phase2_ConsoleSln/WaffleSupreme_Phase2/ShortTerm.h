#pragma once

#include <queue>
#include <cstring>

#include "Process.h"
#include "LongTerm.h"
#include "Dispatcher.h"
#include "CPU.h"

using namespace std;

class ShortTerm {
public:
	ShortTerm(queue<Process*>* zeQ, LongTerm* sched, CPU* target) :
		zeQueue(zeQ), scheduler(sched), targetCPU(target) {}
	bool RunNextProcess();

private:
	LongTerm * scheduler;
	CPU* targetCPU;
	queue<Process*>* zeQueue;
	int cpuID;
	string outFileName = "JobOutputFile.txt";
};
