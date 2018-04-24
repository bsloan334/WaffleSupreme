#pragma once

#include <queue>
#include <cstring>
#include <cstdlib>
#include <thread>
#include <sstream>
#include <iostream>

#include "Process.hpp"
#include "LongTerm.hpp"
#include "Dispatcher.hpp"
#include "CPU.hpp"
#include "RAM.hpp"
#include "Mutex.hpp"
#include "Types.hpp"

using namespace std;

class ShortTerm{
public:
	ShortTerm(LongTerm* sched, CPU* target, MMU* mmu);
	void RunProcesses();
	void PrintSummary() { cout << output.str(); }

private:
	LongTerm* scheduler;		// Access to the LongTerm Scheduler (This is shared between CPUs)
	CPU* targetCPU;				// CPU to use
	MMU* mmu;					// Access needed in order to deallocate finished processes

	stringstream output;
};