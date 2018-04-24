#include "stdafx.h"
#include "Statistics.h"
#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <stdio.h>
#include <time.h>
#include <ctime>
#include <ratio>


using namespace std;
using std::chrono::system_clock;
using std::chrono::nanoseconds;
using std::chrono::duration_cast;

Statistics::~Statistics()
{
}

void Statistics::SetStats(int statType, int index, chrono::time_point<chrono::system_clock> startT, chrono::time_point<chrono::system_clock> endT, bool start)
{
	statStruct stats;
	stats.processID = index;
	stats.start = startT;
	stats.end = endT;

	switch (statType)
	{
		// WAITING case
	case 0:
		if (start == true)
		{
			waitingVector.push_back(stats);
		}
		else
		{
			statStruct *temp = &waitingVector[index];
			temp->end = stats.end;
			waitingVector[index] = *temp;
		}
		break;

		// TURNAROUND case
	case 1:
		if (start == true)
		{
			turnaroundVector.push_back(stats);
		}
		else
		{
			statStruct *temp = &turnaroundVector[index];
			temp->end = stats.end;
			turnaroundVector[index] = *temp;
		}
		break;

		// RESPONSE case
	case 2:
		if (start == true)
		{
			responseVector.push_back(stats);
		}
		else
		{
			statStruct *temp = &responseVector[index];
			temp->end = stats.end;
			responseVector[index] = *temp;
		}
		break;
	// RUNNING case
	case 3:
		if (start == true)
		{
			runningVector.push_back(stats);
		}
		else
		{
			statStruct *temp = &runningVector[index];
			temp->end = stats.end;
			runningVector[index] = *temp;
		}
		break;
	}
}
void Statistics::PrintStats()
{
	int waitingTotal = 0;
	cout << "Printing Statistics Array\n--------------------------\n";
	cout << "Waiting Statistics:" << endl;
	for (int i = 0; i < 30; i++)
	{
		const auto duration = duration_cast<nanoseconds>(waitingVector[i].end - waitingVector[i].start).count();
		cout << "Process " << i + 1 << " waiting time: " << duration << " nanoseconds" << endl;
	}
	cout << "---------------------------" << endl;
	cout << "Turnaround Statistics:" << endl;
	for (int j = 0; j < 30; j++)
	{
		const auto duration = duration_cast<nanoseconds>(turnaroundVector[j].end - turnaroundVector[j].start).count();
		cout << "Process " << j + 1 << " turnaround time: " << duration << " nanoseconds" << endl;
	}
	cout << "---------------------------" << endl;
	cout << "Response Statistics" << endl;
	for (int k = 0; k < 30; k++)
	{
		const auto duration = duration_cast<nanoseconds>(responseVector[k].end - responseVector[k].start).count();
		cout << "Process " << k + 1 << " response time " << duration << " nanoseconds" << endl;
	}
	cout << "---------------------------" << endl;
	cout << "Running Statistics:" << endl;
	for (int a = 0; a < 30; a++)
	{
		const auto duration = duration_cast<nanoseconds>(responseVector[a].end - responseVector[a].start).count();
		cout << "Process " << a + 1 << " running time " << duration << " nanoseconds" << endl;
	}
	cout << "End Statistics Output\n-------------------------------";
}