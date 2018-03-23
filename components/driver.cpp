#pragma once

#include <cstdlib>
#include <iostream>
#include <cstring>
#include <queue>

#include "CPU.hpp"
#include "Loader.hpp"
#include "PCBManager.hpp"
#include "Process.hpp"
#include "Types.hpp"

using namespace std;

int main(int argc, char* argv[])
{
	cout << "Status: Compiled Sucessfully." << endl;

	/*** Initialize all data structures ***************/
	PCBManager pcb;		// Empty Process Control Block
	RAM ram;			// Random Access Memory
	Disk disk;			// Simulated Disk

	// TODO: Pass in queues to appropriate modules for initialization
	queue<Process*> newQueue;		 // Initialized by Loader
	queue<Process*> readyQueue;		 // Initialized by LongTermSched
	queue<Process*> outputQueue;	 // Initialized by ShortTermSched

	/*** Initialize other modules ***/
	int cpuID = 1;
	string jobFile = "JobFile.txt";

	CPU cpu = CPU(&ram, cpuID);			     // CPU (id=1)
	Loader loader = Loader(&disk, &pcb, &newQueue);
	//LongTerm...
	

	/*** Load processes into Disk ***/
	loader.LoadJobs(jobFile);
		/*
		/*** simulate Long Term by moving first process to RAM 
		Process* p = newQueue.front();
		int ramAddress = 0;							// Load process in one at a time starting at 0 (only for now)
		int originalProgBase = p->GetProgramBase();
		instruction_t instr = disk.ReadInstruction(originalProgBase);
		int newProgBase = ram.Allocate(instr, ramAddress);

		for (int i = 1; i < p->GetFullProgramSize(); i++)
		{
			ramAddress += 4;
			instr = disk.ReadInstruction(originalProgBase + i*4);
			ram.Allocate(instr, ramAddress);
		}

		p->SetProgramBase(newProgBase);
		p->AssignInputBase(newProgBase + p->GetProgramSize());
		p->AssignOutputBase(p->GetInputBase() + p->GetInputSize());
		p->AssignTempBase(p->GetOutputBase() + p->GetOutputSize());

		p->SetState(READY);
		readyQueue.push(p);
		newQueue.pop();
		*/
	/***
	This section mimics what the LongTerm, ShortTerm, and Dispatcher will accomplish once incorporated
	***/

	cout << "new Q size: " << newQueue.size() << endl;

	while (!readyQueue.empty())
	{
		Process* process = readyQueue.front();
		cpu.RunProcess(process);
		readyQueue.pop();
	}

	/*****************************/


	return EXIT_SUCCESS;
}
