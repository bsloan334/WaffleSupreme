#pragma once

#include <iostream>
#include <list>
#include <stdio.h>
#include <time.h>
#include <ctime>
#include <ratio>
#include <chrono>
#include <vector>

using namespace std;

struct statStruct
{
	int processID;
	chrono::time_point<chrono::system_clock> start;
	chrono::time_point<chrono::system_clock> end;

	statStruct()
		: processID(0),
		start(std::chrono::system_clock::now()),
		end(std::chrono::system_clock::now())
	{}
};

struct memoryStruct
{
	int processID;
	float blocksUsed;
	float percentage;
};

class Statistics
{
public:
	Statistics()
	{
	}
	~Statistics();
	void SetStats(int statType, int pid, chrono::time_point<chrono::system_clock> startT, chrono::time_point<chrono::system_clock> endT, bool start);
	void PrintStats();
	vector<statStruct> waitingVector;
	vector<statStruct> turnaroundVector;
	vector<statStruct> responseVector;
	vector<statStruct> runningVector;

	memoryStruct * memUsage = new memoryStruct[30];
private:

};

