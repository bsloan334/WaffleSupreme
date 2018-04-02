#include "ShortTerm.hpp"
#include <queue>

using namespace std;

bool ShortTerm::RunNextProcess()
{
	Process* p;
	Dispatcher dispatch;
	bool processRun = false;
	
	if (!zeQueue->empty())
	{
		p = zeQueue->front();	// WARNING: Critical section not properly handled
		zeQueue->pop();
		dispatch.LoadProcessToCPU(p, targetCPU);
		processRun = true;
	
	}
	else
		processRun = false;

	return processRun;
}
