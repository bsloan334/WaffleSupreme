#pragma once

#include <cstdlib>
#include <cstdint>
using namespace std;

//
// Record of process time requirements, priority, and remaining process time used for CPU scheduling
//
class Schedule
{
public: 
	Schedule(int32_t burst, int32_t p, string queue, int32_t slice, int32_t remain) :
		burstTime(burst),
		priority(p),
		queueType(queue),
		timeSlice(slice),
		timeRemaining(remain)
	{
	
	}
private: 
	int32_t burstTime; 
	int32_t priority;
	string queueType;
	int32_t timeSlice;
	int32_t timeRemaining;
};