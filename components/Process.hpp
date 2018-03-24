#pragma once

#include "Job.hpp"
#include "Data.hpp"
#include "Types.hpp"

#include <cstdlib>
#include <iostream>
#include <list>
#include <cstdint>


/** Process state constants **/
#define NEW        1
#define READY      2
#define RUNNING    3
#define WAITING    4
#define TERMINATED 5

/** Other constants **/
#define NBR_OF_REGS 16


class Process
{
    public:
        Process(int id, 
                int instrSetSize, 
                int priority, 
                int inputSize, 
                int outputSize, 
                int tempSize) :
                job(id, instrSetSize, priority),
                data(inputSize, outputSize, tempSize)
        {
            registers = new instruction_t[NBR_OF_REGS]; // create empty register array
            currentState = NEW;
            programCounter = new int(0);

			for (int i = 0; i < NBR_OF_REGS; i++)
				*(registers + i) = 9;
		}
	
		Process(const Process &p)
		{
			currentState = p.currentState;
			programCounter = new int(*(p.programCounter));
			registers = new instruction_t[NBR_OF_REGS];
			programBase = p.programBase;
			inputBase = p.inputBase;
			outputBase = p.outputBase;
			tempBase = p.tempBase;
			job = Job(p.job);
			data = Data(p.data);
			fullProgramSize = p.fullProgramSize;

			for (int i = 0; i < NBR_OF_REGS; i++)
				*(registers + i) = *(p.registers + i);
		}

		~Process() 
		{
			delete [] registers;                      // deallocate registers
			delete programCounter;
		}
    
		//int cpuid = 0;							// Integer index of assigned cpu
		//State *state;									// State struct object to hold process environment at given time				
		//Schedule sched;								// Schedule struct object to hold process time requirement and priority
		//Accounts accounting;
		//Process *Parent;

        int CheckState() { return currentState; }
		void SetState(int st) {
			if (st == NEW || st == READY || st == WAITING || st == RUNNING || st == TERMINATED)
				currentState = st;
			else
				std::cout << "State was not set. " << st << " is invalid state value." << std::endl;
		}
    
        // Provides CPU direct read/write access to Registers and PC
        instruction_t* Registers() { return registers; }
        int* ProgramCounter() { return programCounter; }
	
        // Set locations in Disk/RAM of different program sections
        int GetProgramBase() { return programBase; }
        int GetInputBase() { return inputBase; }
        int GetOutputBase() { return outputBase; }
        int GetTempBase() { return tempBase; }

		// Get fields from Job and Data
		size_t GetProgramSize() { return job.GetProgramSize(); }
		size_t GetInputSize() { return data.GetInputBufferSize(); }
		size_t GetOutputSize() { return data.GetOutputBufferSize(); }
		size_t GetTempSize() { return data.GetTempBufferSize(); }
		size_t GetFullProgramSize() { return fullProgramSize; }
		
		// Other process properties
		int GetID() { return job.GetProgramID(); }

        void SetProgramBase(int address) { programBase = address; }
        void AssignOutputBase(int oBase) { outputBase = oBase; }
        void AssignTempBase(int tBase) { tempBase = tBase; }
        void AssignInputBase(int iBase) { inputBase = iBase; }
		void SetFullProgramSize(size_t size) { fullProgramSize = size; }



    private:
        instruction_t* registers;			// 1D array of registers (no buffers) for program data
   
        Job job;
        Data data;
    
        int currentState;                             // By default, state is NEW
        int* programCounter;                          // Integer offset from programBase
    
        int programBase = 0;                        // Absolute address in RAM of program start
        int inputBase = 0;                          // Offset from programBase
        int outputBase = 0;                         // Offset from programBase
        int tempBase = 0;                           // Offset from programBase

		size_t fullProgramSize = 0;
};
