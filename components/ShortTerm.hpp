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
	   ShortTerm(queue<Process*>* zeQ, LongTerm* sched, CPU* target) :
		   zeQueue(zeQ), scheduler(sched), targetCPU(target) {}
	  bool RunNextProcess();
   
   private:
	   LongTerm* scheduler;
	   CPU* targetCPU;
	   queue<Process*>* zeQueue;
	   int cpuID;
};