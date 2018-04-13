#include "stdafx.h"
#include "Dispatcher.h"

using namespace std;

void Dispatcher::LoadProcessToCPU(Process* p, CPU* targetCPU)
{
	p->SetState(RUNNING);
	targetCPU->RunProcess(p);
	// print process output here?
}
