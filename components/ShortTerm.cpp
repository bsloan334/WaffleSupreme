#include "ShortTerm.hpp"
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
	Process* p = NULL;
	queue<Process*> ps;

	while ((p = scheduler->GetNextProcess()) != NULL)
	{
		Dispatcher d;
		d.LoadProcessToCPU(p, targetCPU, mmu);
		ps.push(p);
	}

	// PRINT all processes that this scheduler executed
	while (!ps.empty())
	{
		Process* pro = ps.front();
		output << "CPU " << targetCPU->GetID() << " processed P " << pro->GetID() << endl;
		ps.pop();
	}
}