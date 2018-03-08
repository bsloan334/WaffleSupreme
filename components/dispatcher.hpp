#pragma once
#include <PCBManager.hpp>

class Dispatcher
{
public: 
	Dispatcher()
	{

	}

	Dispatcher()
	{

	}

	void LoadProcessToCPU(PCBManager* PCB, int PID, CPU targetCPU);
	// Precondition:     PID is an existing ID of an instruction
	//					 in the ready queue
	// Postcondition:	 Dispatcher sets the process object for the
	//					 CPU, along with program counter and relevant registers
private:

};