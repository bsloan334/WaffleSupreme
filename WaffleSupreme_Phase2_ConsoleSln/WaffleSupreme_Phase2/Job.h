#pragma once

#include <cstdlib>
#include <iostream>
#include <cstdint>
using namespace std;

class Job
{
public:
	Job(int id = 0, int instrSetSize = 0, int p = 0)
	{
		programID = new int(id);
		programSize = new int(instrSetSize);
		priority = new int(p);
	}

	Job(const Job &j)
	{
		programID = new int(*(j.programID));
		programSize = new int(*(j.programSize));
		priority = new int(*(j.priority));
	}

	int GetProgramID() { return *programID; }
	int GetProgramSize() { return *programSize; };
	int GetPriority() { return *priority; }
	void AssignPriority(int p) { *priority = p; }

private:
	int *programID;
	int *programSize;
	int *priority;
};
