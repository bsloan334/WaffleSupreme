#pragma once
#include "Process.hpp"
#include "types.hpp"
#include "circularBuffer.hpp"
#include "register.hpp"

#include <cstdlib>
#include <iostream>
#include <deque>
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
	deque<Process*> PCB;

	Process* GetPCBHead();
	int GetSize();
	Process* FindProcess(int index);
	void AddProcess(Process newProcess);
	//void RemoveProcess(Process target);
	//void SortPCB(int schedulingType);
private:
	int size = 0;
	Process* listHead;
};

