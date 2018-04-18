#include <cassert>
#include "LongTerm.hpp"

using namespace std;

LongTerm::~LongTerm() {

}

/*** Move first Process p in zeQueue from Disk to RAM ***/
bool LongTerm::FillZeQueue() {
    
	bool ramFull = false;
	b_address_t ramProgramBase;
	Process* p;

	do
	{
		/*** Get next process from PCB based on scheduling type ***/
		if (scheduleType == FIFO)
			p = FirstProcessByArrival();
		else if (scheduleType == PRIORITY)
			p = FirstProcessByPriority();
		else
		{	cout << "Invalid scheduling type, not FIFO or PRIORITY" << endl;
			p = NULL;
		}

		/*** Put that process into RAM, if room ***/
		if (p != NULL)
		{
			queue<instruction_t> instrs = disk->ReadInstructionChunk(p->GetProgramBase(), p->GetProgramSize());
			ramProgramBase = ram->AllocateChunk(&instrs, p->GetID());

			if (ramProgramBase != NULL_ADDRESS)
			{
				zeQueue.push(p);
				p->SetState(READY);
				p->SetProgramBase(ramProgramBase);
			}
			else
				ramFull = true;
		}

	} while (!ramFull && p != NULL);

	return true;
}

Process* LongTerm::GetNextProcess()
// Marches through the PCB looking for Processes marked 'NEW'
//		and adds either the Job with the lowest Job ID or the highest Priority number to ZeQueue
{
	Process* p = NULL;
	
	if (!zeQueue.empty())
	{
		p = zeQueue.front();
		zeQueue.pop();
	}

	return p;
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
		{	p = pcb->CurrentProcess();
			found = true;
		}
	}

	return p;
}