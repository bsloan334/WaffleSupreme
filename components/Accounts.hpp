#pragma once

#include <cstdlib>
#include <cstdint>
using namespace std;

class Accounts
{
public:
	Accounts(int32_t cpuT, int32_t limit, int32_t delay, int32_t start, int32_t end, int32_t io) :
		cpuTime(cpuT),
		timeLimit(limit),
		timeDelay(delay),
		startTime(start),
		endTime(end),
		ioTime(io)
	{

	}
private: 
	int32_t cpuTime;
	int32_t timeLimit;
	int32_t timeDelay;
	int32_t startTime;
	int32_t endTime;
	int32_t ioTime;
};
