#include <cstdlib>
#include <iostream>
#include <cstring>
#include <queue>
#include <thread>

#include "CPU.hpp"
#include "Loader.hpp"
#include "PCBManager.hpp"
#include "Process.hpp"
#include "LongTerm.hpp"
#include "ShortTerm.hpp"
#include "Types.hpp"

using namespace std;

void RunThread(ShortTerm** stScheds, int index);

int main(int argc, char* argv[])
{
	cout << "Status: Compiled Sucessfully." << endl;

	/*** Initialize all data structures ***************/
	PCBManager pcb;		// Empty Process Control Block
	RAM ram;			// Random Access Memory
	Disk disk;			// Simulated Disk

	/*** Initialize other modules ***/
	string jobFile = "JobFile.txt";

	/*** Initialize CPUs ***/
	CPU* processors[CPU_NBR];
	for (int i = 0; i < CPU_NBR; i++)
		processors[i] = new CPU(&ram, i);

	/*** Initialize Loader and Load jobs into Disk ***/
	Loader loader = Loader(&disk, &pcb);
	loader.LoadJobs(jobFile);			 // Load jobs into Disk

	/*** Initialize Long Term Scheduler ***/
	LongTerm longTermSched(&ram, &disk, &pcb, FIFO);

	/*** Initialize array of Short Term Schedulers, one for each CPU ***/
	ShortTerm* shortTermScheds[CPU_NBR];
	for (int i = 0; i < CPU_NBR; i++)
		shortTermScheds[i] = new ShortTerm(&longTermSched, processors[i]);

	/*** Array of thread pointers ***/
	thread* t[CPU_NBR];

	/*** Fill zeQueue and run processes on different CPUs ***/
	while ( longTermSched.FillZeQueue() == true )
	{
		for (int i = 0; i < CPU_NBR; i++)
			t[i] = new thread(RunThread, shortTermScheds, i);
	}

	for (int i = 0; i < CPU_NBR; i++)
	{
		t[i]->join();
	}

	for (int i = 0; i < CPU_NBR; i++)
	{
		cout << "Sched " << i << endl;
		shortTermScheds[i]->printOutput();
		processors[i]->printOutput();
	}

	return EXIT_SUCCESS;
}

void RunThread(ShortTerm** stScheds, int index)
{
	//cout << "Running processes from zeQueue on scheduler " << index << endl;
	stScheds[index]->RunProcesses();
}