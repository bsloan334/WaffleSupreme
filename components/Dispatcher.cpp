#include "Dispatcher.hpp"

using namespace std;

void Dispatcher::LoadProcessToCPU(Process* p, CPU* targetCPU, RAM* ram)
{
	p->SetState(RUNNING);
	targetCPU->RunProcess(p);
	p->SetState(TERMINATED);
	ram->Deallocate(p->GetProgramBaseRAM(), p->GetProgramSize()*WORD);
}