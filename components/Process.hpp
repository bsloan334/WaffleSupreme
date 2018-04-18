
#pragma once

#include "Job.hpp"
#include "Data.hpp"
#include "Cache.hpp"
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
			delete cache;
		}

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
	
        // Program Base meta data retrieved or derived from Job File
        b_address_t GetProgramBase() { return programBase; }
		b_address_t GetProgramEnd() { return programBase + WORD*GetProgramSize(); }
		i_size_t GetProgramSize() { return job.GetProgramSize(); }

		// Get/Set process's cache
		Cache* GetCache() { return cache; };
		void SetCache(Cache* c) { cache = c; }
		
		// Other process properties
		int GetID() { return job.GetProgramID(); }
		int GetPriority() { return job.GetPriority(); }

        void SetProgramBase(b_address_t address) { programBase = address; }


    private:
        instruction_t* registers;			// 1D array of registers (no buffers) for program data
   
        Job job;
        Data data;
		Cache* cache;
    
        int currentState;                             // By default, state is NEW
        b_address_t* programCounter;                  // Integer offset from programBase
    
		
        b_address_t programBase = 0;                // Absolute address in RAM of program start

};