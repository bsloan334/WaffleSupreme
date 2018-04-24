#include "stdafx.h"
#include "ShortTerm.h"
#include "Dispatcher.h"
#include "Statistics.h"

#include <queue>

using namespace std;

bool ShortTerm::RunNextProcess()
{
	statStruct *turnaround = new statStruct;
	Process* p;
	
	Dispatcher dispatch;
	bool processRun = false;

	if (!zeQueue->empty())
	{
		p = zeQueue->front();	// WARNING: Critical section not properly handled

		stats->SetStats(1, p->GetID(), std::chrono::system_clock::now(), std::chrono::system_clock::now(), true);

		zeQueue->pop();

		stats->SetStats(2, p->GetID(), std::chrono::system_clock::now(), std::chrono::system_clock::now(), true);
		dispatch.LoadProcessToCPU(p, targetCPU);
		stats->SetStats(2, p->GetID()-1, std::chrono::system_clock::now(), std::chrono::system_clock::now(), false);
		processRun = true;

		stats->SetStats(1, p->GetID()-1, std::chrono::system_clock::now(), std::chrono::system_clock::now(), false);
	}
	else
		processRun = false;

	return processRun;
}
