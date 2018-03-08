//#include "stdafx.h"
#include "PCBManager.hpp"
#include <iostream>

using namespace std;

Process* PCBManager::GetPCBHead()
{
	*listHead = PCB.front();

	return listHead;
}

int PCBManager::GetSize()
{
	return size;
}

Process* PCBManager::FindProcess(int index)
{
	if (PCB.size > 0)
	{
		return PCB[index];
	}
	else
		return PCB.front();
}

void PCBManager::AddProcess(Process newProcess)
{
	*PCB.push_back(newProcess);
	size++;
}
