
#pragma once

#include "Job.hpp"
#include "Data.hpp"
#include "Types.hpp"

#include <cstdlib>
#include <iostream>
#include <list>
#include <cstdint>

/** Other constants **/
#define NBR_OF_REGS 16

enum State{ NEW, READY, RUNNING, WAITING, TERMINATED };

class Process
{
    public:
        Process(int id, 
                i_size_t instrSetSize, 
                int priority, 
                i_size_t inputSize, 
                i_size_t outputSize, 
                i_size_t tempSize) :
                job(id, instrSetSize, priority),
                data(inputSize, outputSize, tempSize)
        {
            registers = new instruction_t[NBR_OF_REGS]; // create empty register array
            currentState = NEW;
            programCounter = new b_address_t(0);
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
        b_address_t* ProgramCounter() { return programCounter; }
	
        // Set locations in Disk/RAM of different program sections
        b_address_t GetProgramBase() { return programBase; }
		b_address_t GetProgramEnd() { return programBase + WORD*GetFullProgramSize(); }
        b_address_t GetInputBase() { return programBase + WORD*GetProgramSize(); }
        b_address_t GetOutputBase() { return GetInputBase() + WORD*GetInputSize(); }
        b_address_t GetTempBase() { return GetOutputBase() + WORD*GetOutputSize(); }

		// Get fields from Job and Data
		i_size_t GetProgramSize() { return job.GetProgramSize(); }
		i_size_t GetInputSize() { return data.GetInputBufferSize(); }
		i_size_t GetOutputSize() { return data.GetOutputBufferSize(); }
		i_size_t GetTempSize() { return data.GetTempBufferSize(); }
		i_size_t GetFullProgramSize() {
			return GetProgramSize() + GetInputSize() + GetOutputSize() + GetTempSize();
		}
		
		// Other process properties
		int GetID() { return job.GetProgramID(); }
		int GetPriority() { return job.GetPriority(); }

        void SetProgramBase(b_address_t address) { programBase = address; }
        //void AssignOutputBase(int oBase) { outputBase = oBase; }
        //void AssignTempBase(int tBase) { tempBase = tBase; }
        //void AssignInputBase(int iBase) { inputBase = iBase; }
		//void SetFullProgramSize(size_t size) { fullProgramSize = size; }

		// Mass refactor functions
		/*
		void RefactorSectionBases()
		{
			inputBase = programBase + job.GetProgramSize();
			outputBase = inputBase + data.GetInputBufferSize();
			tempBase = outputBase + data.GetOutputBufferSize();
		}
		*/


    private:
        instruction_t* registers;			// 1D array of registers (no buffers) for program data
   
        Job job;
        Data data;
    
        int currentState;                             // By default, state is NEW
        b_address_t* programCounter;                          // Integer offset from programBase
    
		
        b_address_t programBase = 0;                        // Absolute address in RAM of program start
		/*
		int inputBase = 0;                          // Offset from programBase
        int outputBase = 0;                         // Offset from programBase
        int tempBase = 0;                           // Offset from programBase
		*/

		//size_t fullProgramSize = 0;
};
