#include "PCBManager.hpp"

#include <iostream>
#include <cassert>

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
	for (int i = 0; i < index; i++)
		itr++;

	return *itr;
}

void PCBManager::Start()
{
	assert(PCB.size() > 0);
	itr = PCB.begin();
}

void PCBManager::Next()
{
	assert(itr != PCB.end());
	itr++;
}

bool PCBManager::AtEnd()
{
	return (itr == PCB.end());
}

Process* PCBManager::CurrentProcess()
{
	return *itr;
}