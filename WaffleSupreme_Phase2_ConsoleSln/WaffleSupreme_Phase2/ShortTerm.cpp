#include "stdafx.h"
#include "ShortTerm.h"
#include "Statistics.h"
#include <queue>

using namespace std;

bool ShortTerm::RunNextProcess()
{
	Statistics stats;
	Process* p;
	Dispatcher dispatch;
	bool processRun = false;

	if (!zeQueue->empty())
	{
		p = zeQueue->front();	// WARNING: Critical section not properly handled
		stats.AddStats(1, p->GetID(), true);
		stats.AddStats(2, p->GetID(), true);
		zeQueue->pop();
		dispatch.LoadProcessToCPU(p, targetCPU);
		processRun = true;

		stats.AddStats(1, p->GetID(), false);
	}
	else
		processRun = false;

	return processRun;
}
