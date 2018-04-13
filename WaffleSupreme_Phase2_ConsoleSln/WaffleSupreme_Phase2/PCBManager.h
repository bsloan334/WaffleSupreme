#pragma once

#include "Process.h"
#include "Types.h"

#include <cstdlib>
#include <iostream>
#include <list>
#include <cstdint>

using namespace std;

class PCBManager
{
public:
	PCBManager()
	{
	}

	~PCBManager()
	{
	}

	Process* FindProcess(int index);
	int GetSize();
	void AddProcess(Process* newProcess);

	void RemoveProcess(Process* target);
	void SortPCB(int schedulingType);

private:
	list<Process*> PCB;
};

