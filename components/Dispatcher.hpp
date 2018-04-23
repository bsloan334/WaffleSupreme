#pragma once

#include "Process.hpp"
#include "CPU.hpp"
#include "RAM.hpp"

class Dispatcher
{
public:
	Dispatcher() {};

	void LoadProcessToCPU(Process* p, CPU* targetCPU, RAM* ram);
	// Precondition:     PID is an existing ID of an instruction
	//					 in the ready queue
	// Postcondition:	 Dispatcher sets the process object for the
	//					 CPU, along with program counter and relevant registers

};