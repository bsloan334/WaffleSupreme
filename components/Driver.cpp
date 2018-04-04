#include <cstdlib>
#include <iostream>
#include <cstring>
#include <queue>

#include "CPU.hpp"
#include "Loader.hpp"
#include "PCBManager.hpp"
#include "Process.hpp"
#include "LongTerm.hpp"
#include "ShortTerm.hpp"
#include "Types.hpp"

using namespace std;

void PrintOutput(Process* p, RAM* ram);

int main(int argc, char* argv[])
{
	cout << "Status: Compiled Sucessfully." << endl;

	/*** Initialize all data structures ***************/
	PCBManager pcb;		// Empty Process Control Block
	RAM ram;			// Random Access Memory
	Disk disk;			// Simulated Disk

	/*** Initialize other modules ***/
	string jobFile = "JobFile.txt";

	queue<Process*> newQueue;
	queue<Process*> zeQueue;

	CPU cpu = CPU(&ram, 1);			     // CPU (id=1)
	Loader loader = Loader(&disk, &pcb, &newQueue);
	loader.LoadJobs(jobFile);			 // %Load jobs into Disk

	LongTerm longTermSched(&newQueue, &zeQueue, &ram, &disk, &pcb);
	longTermSched.ScheduleJobs(FIFO);

	ShortTerm shortTermSched(&zeQueue, &longTermSched, &cpu);
	// Takes the first process on zeQueue (ready queue) and
	//   executes process by calling dispatcher

	while ( longTermSched.FillZeQueue() == true )
	{
		ram.printAvailableSpace();
		while ( shortTermSched.RunNextProcess() == true )
			ram.printAvailableSpace();
	}

	return EXIT_SUCCESS;
}
