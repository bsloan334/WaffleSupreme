#include "PCBManager.hpp"

#include <iostream>

using namespace std;

Process* PCBManager::GetPCBHead()
{
	listHead = &PCB.front();

	return listHead;
}

void PCBManager::AddProcess(Process newProcess)
{
	PCB.push_back(newProcess);
	size++;
}

void PCBManager::RemoveProcess(Process p)
{
	if (&p == listHead)
	{
		PCB.remove(p);
		listHead = &PCB.front();
	}
	else
	{
		PCB.remove(p);
	}

	size = PCB.size();
}

int PCBManager::GetSize()
{
	return size;
}

Process* PCBManager::FindProcess(int index)
{

	if (size == 0)
		return NULL;

	list<Process>::const_iterator itr = PCB.cbegin();
	for (int i = 0; i < size; i++)
		itr++;

	Process p = *itr;

	return &p;
}
