#include "Dispatcher.hpp"

using namespace std;

void Dispatcher::LoadProcessToCPU(Process* p, CPU* targetCPU, MMU* mmu)
{
	p->SetState(RUNNING);
	targetCPU->RunProcess(p);
	p->SetState(TERMINATED);
	mmu->Deallocate(p);
}