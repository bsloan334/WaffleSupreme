#include "stdafx.h"
#include "Dispatcher.h"
#include "Statistics.h"
using namespace std;

void Dispatcher::LoadProcessToCPU(Process* p, CPU* targetCPU)
{
	Statistics stats;

	p->SetState(RUNNING);
	stats.AddStats(2, p->GetID(), false);
	targetCPU->RunProcess(p);
	// print process output here?
}
