#include <cstdlib>
#include <iostream>
#include <cstring>
#include <queue>

#include "stdafx.h"
#include "CPU.h"
#include "Loader.h"
#include "PCBManager.h"
#include "Process.h"
#include "LongTerm.h"
#include "ShortTerm.h"
#include "Types.h"
#include "Statistics.h"
using namespace std;

void PrintOutput(Process* p, RAM* ram);

int main(int argc, char* argv[])
{
	cout << "Status: Compiled Sucessfully." << endl;

	/*** Initialize all data structures ***************/
	PCBManager pcb;		// Empty Process Control Block
	RAM ram;			// Random Access Memory
	Disk disk;			// Simulated Disk
	Statistics stats;	// Statistics calculator

						/*** Initialize other modules ***/
	string jobFile = "JobFile.txt";

	queue<Process*> newQueue;
	queue<Process*> zeQueue;

	CPU cpu = CPU(&ram, &stats, 1);			     // CPU (id=1)
	Loader loader = Loader(&disk, &pcb, &newQueue, &stats);
	loader.LoadJobs(jobFile);			 // %Load jobs into Disk

	LongTerm longTermSched(&newQueue, &zeQueue, &ram, &disk, &pcb);
	longTermSched.ScheduleJobs(FIFO);

	ShortTerm shortTermSched(&zeQueue, &longTermSched, &cpu, &stats);
	// Takes the first process on zeQueue (ready queue) and
	//   executes process by calling dispatcher

	while (longTermSched.FillZeQueue() == true)
	{
		ram.printAvailableSpace();
		while (shortTermSched.RunNextProcess() == true)
			ram.printAvailableSpace();
	}

	stats.PrintStats();
	return EXIT_SUCCESS;
}
