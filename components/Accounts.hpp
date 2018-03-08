#pragma once

#include <cstdlib>
#include <cstdint>
using namespace std;

class Accounts
{
public:
	Accounts()
	{

	}
	Accounts(int cpuT, int limit, int delay, int start, int end, int io) :
		cpuTime(cpuT),
		timeLimit(limit),
		timeDelay(delay),
		startTime(start),
		endTime(end),
		ioTime(io)
	{

	}
private: 
	int cpuTime;
	int timeLimit;
	int timeDelay;
	int startTime;
	int endTime;
	int ioTime;
};
