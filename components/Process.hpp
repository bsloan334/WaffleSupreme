
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
        Process()
        {

        }
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
    
        // Provides CPU direct read/write access to Registers and PC
        instruction_t* Registers() { return registers; }
        int* ProgramCounter() { return programCounter; }
	
        // Set locations in Disk/RAM of different program sections
        int GetProgramBase() { return programBase; }
        int GetInputBase() { return inputBase; }
        int GetOutputBase() { return outputBase; }
        int GetTempBase() { return tempBase; }

        void SetProgramBase(int address) { programBase = address; }
        void AssignOutputBase(int oBase) { outputBase = oBase; }
        void AssignTempBase(int tBase) { tempBase = tBase; }
        void AssignInputBase(int iBase) { inputBase = iBase; }

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
};
