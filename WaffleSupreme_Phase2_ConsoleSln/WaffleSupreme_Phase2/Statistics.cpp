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

void Statistics::AddStats(int statType, int pid, bool startCheck)
{
/*	switch (statType)
	{
	// WAITING case
	case 0:	
		if (startCheck == true)
		{
			waiting[pid-1].processID = pid;
			const auto temp = system_clock::now();
			waiting[pid-1].start = temp;
		}
		else
		{
			auto temp = std::chrono::system_clock::now();
			waiting[pid-1].end = temp;
		}
		break;
	// TURNAROUND case
	case 1:
		if (startCheck == true)
		{
			turnaround[pid-1].processID = pid;
			auto temp = std::chrono::system_clock::now();
			turnaround[pid-1].start = temp;
		}
		else
		{
			auto temp = std::chrono::system_clock::now();
			turnaround[pid-1].end = temp;
		}
		break;
	// RESPONSE case
	case 2:
		if (startCheck == true)
		{
			response[pid-1].processID = pid;
			auto temp = std::chrono::system_clock::now();
			response[pid-1].start = temp;
		}
		else
		{
			auto temp = std::chrono::system_clock::now();
			response[pid-1].end = temp;
		}
		break;
	}*/
}


void Statistics::SetStats(int statType, int pid, statStruct* wait, bool start)
{
	switch (statType)
	{
		// WAITING case
	case 0:
		if (start == true)
		{
			waiting.push_back(statStruct());
			waiting[pid-1].processID = wait->processID;
			waiting[pid-1].start = wait->start;
		}
		else
			waiting[pid-1].end = wait->end;
		break;

			// TURNAROUND case
	case 1:
		if (start == true)
		{
			turnaround.push_back(statStruct());
			turnaround[pid - 1].processID = wait->processID;
			turnaround[pid - 1].start = wait->start;
		}
		else
			turnaround[pid - 1].end = wait->end;
		break;

		// RESPONSE case
	case 2:
		if (start == true)
		{
			response.push_back(statStruct());
			response[pid - 1].processID = wait->processID;
			response[pid - 1].start = wait->start;
		}
		else
			response[pid - 1].end = wait->end;
		break;
	}
}
void Statistics::PrintStats()
{
/*	cout << "Printing Statistics Array\n--------------------------\n";
	cout << "Waiting Statistics:" << endl;
	for (int i = 0; i <= 30; i++)
	{
		const auto duration = duration_cast<nanoseconds>(waiting[i].end - waiting[i].start).count();
		cout << "Process " << i << ": " << duration	<< " nanoseconds" << endl;
	}
	cout << "---------------------------" << endl;
	cout << "Turnaround Statistcis:" << endl;
	for (int j = 0; j <= 30; j++)
	{
		const auto duration = duration_cast<nanoseconds>(turnaround[j].end - turnaround[j].start).count();
		cout << "Process " << j << ": " << duration << " nanoseconds " << endl;
	}
	cout << "---------------------------" << endl;
	cout << "Response Statistics" << endl;
	for (int k = 0; k <= 30; k++)
	{
		const auto duration = duration_cast<nanoseconds>(response[k].end - response[k].start).count();
		cout << "Process " << k << ": " << duration << " nanoseconds" << endl;
	}
	cout << "End Statistics Output\n-------------------------------";*/
}