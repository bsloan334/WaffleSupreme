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
void DiskDump(PCBManager* pcb);

int main(int argc, char* argv[])
{
	cout << "Status: Compiled Sucessfully." << endl;

	/*** Initialize all data structures ***************/
	PCBManager pcb;		// Empty Process Control Block
	RAM ram;			// Random Access Memory
	Disk disk;			// Simulated Disk
	MMU mmu(&ram, &disk);

	/*** Initialize other modules ***/
	string jobFile = "JobFile.txt";

	/*** Initialize CPUs ***/
	CPU* processors[CPU_NBR];
	for (int i = 0; i < CPU_NBR; i++)
		processors[i] = new CPU(&mmu, i);

	/*** Initialize Loader and Load jobs into Disk ***/
	Loader loader = Loader(&disk, &pcb);
	loader.LoadJobs(jobFile);			 // Load jobs into Disk

	/*** Initialize Long Term Scheduler ***/
	LongTerm longTermSched(&mmu, &pcb, FIFO);

	/*** Initialize array of Short Term Schedulers, one for each CPU ***/
	ShortTerm* shortTermScheds[CPU_NBR];
	for (int i = 0; i < CPU_NBR; i++)
		shortTermScheds[i] = new ShortTerm(&longTermSched, processors[i], &mmu);

	/*** Array of thread pointers ***/
	thread* t[CPU_NBR];

	/*** Fill zeQueue and run processes on different CPUs ***/
	while (longTermSched.FillZeQueue() == true)
	{
		for (int i = 0; i < CPU_NBR; i++)
			t[i] = new thread(RunThread, shortTermScheds, i);
	}

	for (int i = 0; i < CPU_NBR; i++)
	{
		t[i]->join();
	}

	DiskDump(&pcb);

	for (int i = 0; i < CPU_NBR; i++)
		shortTermScheds[i]->PrintSummary();

	return EXIT_SUCCESS;
}

void RunThread(ShortTerm** stScheds, int index)
{
	stScheds[index]->RunProcesses();
}

void DiskDump(PCBManager* pcb)
{
	for (pcb->Start(); !pcb->AtEnd(); pcb->Next())
	{
		Process* p = pcb->CurrentProcess();
		p->GetCache()->PrintOutput();
	}
}