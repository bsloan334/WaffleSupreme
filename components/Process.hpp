#pragma once
#include <Cache.hpp>
#include <State.hpp>
#include <Schedule.hpp>
#include <Permissions.hpp>
#include <CircularBuffer.hpp>
#include <Register.hpp>

#include <cstdlib>
#include <iostream>
#include <list>
#include <cstdint>

class Process
{
public:
	Process()
	{
		int32_t cpuid = 0;								// Integer index of assigned cpu
		int32_t programCounter = 0;						// Integer index of current running program
		int32_t code_size = 0;							// Integer representation of instruction size saved in PCB
		struct state;									// State struct object to hold process environment at given time
		//std::list<register> registers;				// List of registers to hold process quick access data
		Schedule sched;									
		
		// Schedule struct object to hold process time requirement and priority

		Process &Parent();
	}
};