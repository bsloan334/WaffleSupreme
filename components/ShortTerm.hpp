#pragma once

#include <queue>
#include <cstring>

#include "Process.hpp"
#include "LongTerm.hpp"
#include "Dispatcher.hpp"
#include "CPU.hpp"

using namespace std;

class ShortTerm{
   public:
	   ShortTerm(LongTerm* sched, CPU* target) :
		   scheduler(sched), targetCPU(target) {}
	  bool RunNextProcess();
   
   private:
	   LongTerm* scheduler;
	   CPU* targetCPU;
	   int cpuID;
};