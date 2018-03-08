//#include "stdafx.h"
#include <CPU.hpp>

void CPU::AssignProcessData(Process* nextProcess, int programCounter, deque<Register*> *reg)
// Postcondition:   CPU Process* object set to new process ptr along
//					with programCounter and relevant registers
{
	process = nextProcess;
	programCounter = pc;
	
}