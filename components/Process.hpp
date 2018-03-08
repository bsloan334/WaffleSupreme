#pragma once
#include "Cache.hpp"
#include "Permissions.hpp"
#include "CircularBuffer.hpp"
#include "Register.hpp"
#include "Job.hpp"
#include "Data.hpp"
#include "types.hpp"

#include <cstdlib>
#include <iostream>
#include <deque>
#include <stdint.h>

class Process
{
public:
	Process()
	{

	}
	Process(int id, 
			int size, 
			int priority, 
			int inputSize, 
			int outputSize, 
			int tempSize) :
		job(id, size, priority),
		data(inputSize, outputSize, tempSize)
	{
		
	}
	~Process() 
	{

	}
	int *currentState = new int(1);
	int *baseRegister = 0;
	int *programCounter = 0;						// Integer index of current running program
	Job job;
	Data data;

	//int cpuid = 0;							// Integer index of assigned cpu
	//State *state;									// State struct object to hold process environment at given time				
	//Schedule sched;								// Schedule struct object to hold process time requirement and priority
	//Accounts accounting;
	//Process *Parent;

	int CheckState() { return *currentState; }
	int GetBaseRegister() { return *baseRegister; }
	int GetProgramCounter() { return *programCounter; }
	int GetOutputBase() { return *outputBase; }
	int GetTempBase() { return *tempBase; }
	void SetBaseRegister(int value) { *baseRegister = value; }
	void SetProgramCounter(int value) { *programCounter = value; }
	void AssignOutputBase(int oBase) { *outputBase = oBase; }
	void AssignTempBase(int iBase) { *tempBase = iBase; }
	deque<Register*> GetRegisters() {return *registers;}

private:
	deque<Register*> registers[16];						// 2D array of registers and register buffers for program data
	int *outputBase = 0;
	int *tempBase = 0;
};