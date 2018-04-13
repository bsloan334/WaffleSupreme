#pragma once

#include "Process.h"
#include "CPU.h"

class Dispatcher
{
public:
	Dispatcher() {};

	void LoadProcessToCPU(Process* p, CPU* targetCPU);
	// Precondition:     PID is an existing ID of an instruction
	//					 in the ready queue
	// Postcondition:	 Dispatcher sets the process object for the
	//					 CPU, along with program counter and relevant registers

};
