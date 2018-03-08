//#include "stdafx.h"
#include <dispatcher.hpp>
#include <PCBManager.hpp>
#include <Process.hpp>
#include <CPU.hpp>
#include <deque>
#include <iostream>

using namespace std;

void Dispatcher::LoadProcessToCPU(PCBManager* PCB, int PID, CPU targetCPU)
{
	for (int i = 0; i < PCB->GetSize(); i++)
	{
		Process* temp = PCB->FindProcess(PID);
		if (temp->job.GetProgramID() == PID)
		{
			int pc = *temp->programCounter;
			deque<Register*> reg = &temp->GetRegisters();
			targetCPU.AssignProcessData(temp, pc, &reg);
		}
	}
}