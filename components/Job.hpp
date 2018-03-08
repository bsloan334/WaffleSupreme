#pragma once
#include <cstdlib>
#include <iostream>
#include <list>
#include <cstdint>
using namespace std;

class Job
{
public:
	Job()
	{

	}
	Job(int id, int size, int p)
	{
		programID = new int[id];
		programSize = new int[size];
		priority = new int[p];
	}

	int GetProgramID() { return *programID; }
	int GetProgramSize() { return *programSize; };
	int GetPriority() { return *priority; }
	void	AssignPriority(int p) { *priority = p; }

private:
	int *programID;
	int *programSize;
	int *priority;
};