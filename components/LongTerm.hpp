#pragma once

#include "Process.hpp"
#include "PCBManager.hpp"
#include "Disk.hpp"
#include "RAM.hpp"
#include "Mutex.hpp"
#include "MMU.hpp"

#include <queue>
#include <sstream>
#include <iostream>

using namespace std;

enum Scheduler { FIFO, PRIORITY, SJF };

class LongTerm {
public:

	LongTerm(MMU* m, PCBManager* p, int t) :
		mmu(m), pcb(p), scheduleType(t)
	{};
	~LongTerm();

	void ReadyToWait(); //Move process from ready to wait  <-Do we need to do this?
	void WaitToReady(); //Move process from wait to ready  

	// Interface for ShortTerm to call for more processes in RAM
	bool FillZeQueue();
	// Puts as many processes as possible into RAM
	//	Takes them off the newQueue and puts them onto zeQueue

	// Interface for ShortTerm to call execute first process in zeQueue
	Process* GetNextProcess();

	friend ostream& operator << (ostream& o, LongTerm& l) { o << l.out; }


private:

	// Params passed in from Driver
	queue<Process*> zeQueue;
	MMU* mmu;
	PCBManager* pcb;
	int scheduleType;

	Process* FirstProcessByPriority();		/*Returns NEW process with the highest priority*/
	Process* FirstProcessByArrival();			/*Returns NEW process that arrived first*/
	Process* ShortestProcess();				/*Returns NEW process that has the shortest instruction set*/

	stringstream out;
};