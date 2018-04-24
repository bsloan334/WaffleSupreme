#include "ShortTerm.hpp"
#include "Dispatcher.hpp"
#include "Statistics.hpp"
#include <queue>

using namespace std;

ShortTerm::ShortTerm(LongTerm* sched, CPU* target, MMU* m) :
scheduler(sched), targetCPU(target), mmu(m) {}

/******
Logic/Pseudo Code:
- Get next available process from LongTerm
- Dispatch process to desginated CPU
- Continue until there are no processes left in zeQueue

******/
void ShortTerm::RunProcesses()
{
	statStruct *turnaround = new statStruct;
	Process* p = NULL;
	queue<Process*> ps;

	while ((p = scheduler->GetNextProcess()) != NULL)
	{
		Dispatcher d;	
		stats->SetStats(2, p->GetID(), std::chrono::system_clock::now(), std::chrono::system_clock::now(), true);
		d.LoadProcessToCPU(p, targetCPU, mmu);
		stats->SetStats(2, p->GetID()-1, std::chrono::system_clock::now(), std::chrono::system_clock::now(), false);
		ps.push(p);
	}

	// PRINT all processes that this scheduler executed
	while (!ps.empty())
	{
		Process* pro = ps.front();
		stats->SetStats(1, p->GetID(), std::chrono::system_clock::now(), std::chrono::system_clock::now(), true);
		output << "CPU " << targetCPU->GetID() << " processed P " << pro->GetID() << endl;
		ps.pop();
		stats->SetStats(1, p->GetID()-1, std::chrono::system_clock::now(), std::chrono::system_clock::now(), false);
	}
}