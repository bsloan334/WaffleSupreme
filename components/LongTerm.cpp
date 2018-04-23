#include <cassert>
#include "LongTerm.hpp"

using namespace std;

LongTerm::~LongTerm() {

}

/*** Move NEW processes from Disk to RAM so that they are READY processes ***/
bool LongTerm::FillZeQueue() {

	bool ramFull = false;
	bool processesAdded = false;
	b_address_t ramProgramBase;
	Process* p;

	do
	{
		/*** Get next process from PCB based on scheduling type ***/
		if (scheduleType == FIFO)
			p = FirstProcessByArrival();
		else if (scheduleType == PRIORITY)
			p = FirstProcessByPriority();
		else if (scheduleType == SJF)
			p = ShortestProcess();
		else
		{
			//cout << "Invalid scheduling type, not FIFO or PRIORITY" << endl;
			p = NULL;
		}

		/*** If NEW process found, Put that process into RAM, if room ***/
		if (p != NULL)
		{
			queue<instruction_t> instrs = disk->ReadInstructionChunk(p->GetProgramBaseDisk(), p->GetProgramSize());
			ramProgramBase = ram->AllocateChunk(&instrs, p->GetID());

			if (ramProgramBase != NULL_ADDRESS)
			{
				zeQueue.push(p);
				p->SetState(READY);
				p->SetProgramBaseRAM(ramProgramBase);
				processesAdded = true;
			}
			else
				ramFull = true;
		}

	} while (!ramFull && p != NULL);

	//cout << "zeQueue.size() = " << zeQueue.size() << endl;


	/*** Indicates new processes were found in PCB and added to zeQueue ***/
	return processesAdded;
}


Process* LongTerm::GetNextProcess()
// Marches through the PCB looking for Processes marked 'NEW'
//		and adds either the Job with the lowest Job ID or the highest Priority number to ZeQueue
{
	Process* p = NULL;


	//cout << "zeQueue.size() = " << zeQueue.size() << endl;

	while (!zeQueue.empty() && p == NULL)    // Continues until either zeQueue is empty or available process has been found
	{

		p = zeQueue.front();

		assert(p != NULL);

		Mutex* lock = p->GetLock();

		if (lock->TestAndSet() == FREE)		 // Make sure process has not already been claimed before removing if from stack
			zeQueue.pop();
		else                                 // Process has been claimed so set p to NULL and move on to next process in zeQeueu
			p = NULL;
	}

	return p;		// Note: only returns NULL if there are no more processes loaded into RAMs
}

Process* LongTerm::FirstProcessByPriority()		/*Returns NEW process with the highest priority*/
// Returns next process with highest priority OR returns null if no NEW processes are found
{
	assert(pcb->GetSize() > 0);

	int maxPriority = -1;
	Process* maxProcess = NULL;


	pcb->Start();
	for (Process* p = NULL; !pcb->AtEnd(); pcb->Next())		// Iterate through PCB
	{
		p = pcb->CurrentProcess();
		if (p->CheckState() == NEW && p->GetPriority() > maxPriority)
		{
			maxPriority = p->GetPriority();
			maxProcess = p;
		}
	}

	return maxProcess;
}

Process* LongTerm::FirstProcessByArrival() 		/*Returns NEW process that arrived first*/
// Returns next process that arrived first OR returns null if no NEW processes are found
{
	assert(pcb->GetSize() > 0);

	Process* p = NULL;
	bool found = false;
	for (pcb->Start(); !found && !pcb->AtEnd(); pcb->Next())
	{
		if (pcb->CurrentProcess()->CheckState() == NEW)
		{
			p = pcb->CurrentProcess();
			found = true;
		}
	}

	return p;
}

Process* LongTerm::ShortestProcess()	/*Returns NEW process that has the shortest instruction set*/
// Returns the selected process OR returns null if no NEW processes are found
{
	assert(pcb->GetSize() > 0);

	pcb->Start();

	size_t minSize = pcb->CurrentProcess()->GetProgramSize();
	Process* minProcess = NULL;

	for (Process* p = NULL; !pcb->AtEnd(); pcb->Next())		// Iterate through PCB
	{
		p = pcb->CurrentProcess();
		if (p->CheckState() == NEW && p->GetProgramSize() < minSize)
		{
			minSize = p->GetProgramSize();
			minProcess = p;
		}
	}

	return minProcess;
}