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
	void AddStats(int statType, int pid, bool start);
	void SetStats(int statType, int pid, statStruct* wait, bool start);
	void PrintStats();
	//void SetProcessID(int pid);;
	vector<statStruct> waiting;
	vector<statStruct> turnaround;
	vector<statStruct> response;

	//statStruct * waiting = new statStruct[30];
	//statStruct * turnaround = new statStruct[30];
	//statStruct * response = new statStruct[30];
	memoryStruct * memUsage = new memoryStruct[30];
private: 
	
};

