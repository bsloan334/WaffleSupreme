#include "ShortTerm.hpp"
#include <queue>

using namespace std;

bool ShortTerm::RunNextProcess()
{
	Process* p;
	Dispatcher dispatch;
	bool processRun = false;
	
    if ((p = scheduler->GetNextProcess()) != NULL)
	{
		dispatch.LoadProcessToCPU(p, targetCPU);
		processRun = true;
	
	}
	else
		processRun = false;

	return processRun;
}