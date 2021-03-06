#pragma once

#include "Process.h"
#include "PCBManager.h"
#include "Disk.h"
#include "RAM.h"

#include <queue>

using namespace std;

enum Scheduler { FIFO, PRIORITY };

class LongTerm {
public:

	LongTerm(queue<Process*>* newQ,
		queue<Process*>* zeQ,
		RAM* r, Disk* d, PCBManager* p) :
		newQueue(newQ), zeQueue(zeQ),
		ram(r), disk(d), pcb(p)
	{};
	~LongTerm();

	void ReadyToWait(); //Move process from ready to wait  <-Do we need to do this?
	void WaitToReady(); //Move process from wait to ready  

						// Interface for ShortTerm
	void ScheduleJobs(int schedulingType);
	bool FillZeQueue();
	// Puts as many processes as possible into RAM
	//	Takes them off the newQueue and puts them onto zeQueue

private:
	struct BlankSpace {
		i_size_t instructionSize;
		b_address_t startAddress;
		BlankSpace(b_address_t start, i_size_t instructSize) :
			startAddress(start), instructionSize(instructSize) {}
	};

	struct UsedSpace {
		b_address_t begin;
		b_address_t end;
		UsedSpace(b_address_t beg, b_address_t en) : begin(beg), end(en) {}
	};

	// Params passed in from Driver
	queue<Process*>* newQueue;
	queue<Process*>* zeQueue;
	RAM* ram;
	Disk* disk;
	PCBManager* pcb;

	size_t readyQueueSize = 0;
	instruction_t maxAddress = 0;
	//Might not work because it is wantint to use the "used" from disk.hpp not make a new one ****************
	vector<UsedSpace> used;
	vector<BlankSpace> FindBlankSpaces();

	bool IsBlank(BlankSpace blank);
	//bool CheckResource(resourceType rType);



	void printQ()
	{
		queue<Process*> tmp(*zeQueue);
		cout << "Printing zeQueue...";
		while (!tmp.empty())
		{
			cout << " pid: " << tmp.front()->GetID();
			tmp.pop();
		}
		cout << endl;

	}
};
