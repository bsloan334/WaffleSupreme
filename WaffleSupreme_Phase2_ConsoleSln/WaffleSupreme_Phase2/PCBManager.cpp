#include "stdafx.h"
#include "PCBManager.h"

#include <iostream>

using namespace std;

void PCBManager::AddProcess(Process* newProcess)
{
	PCB.push_back(newProcess);
}

void PCBManager::RemoveProcess(Process* p)
{
	PCB.remove(p);
}

int PCBManager::GetSize()
{
	return PCB.size();
}

Process* PCBManager::FindProcess(int index)
{

	if (GetSize() == 0)
		return NULL;

	list<Process*>::iterator itr = PCB.begin();
	for (int i = 0; i < GetSize(); i++)
		itr++;

	return *itr;
}
