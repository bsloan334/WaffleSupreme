#pragma once
#include <iostream>
#include "Loader.hpp"

using namespace WaffleSupreme;
using namespace std;

const string Loader::JOB_START;
const string Loader::DATA_START;
const string Loader::JOB_END;
const size_t Loader::MAX_LINE_LEN;

void Loader::LoadJobs(string jobSrcFile, Process* pcb_head)
// Postcondition:	Jobs from src file have been loaded into memory
//					and stored as Process entries in the PCB
{
	// Initialize file reader and open file
	ifstream jobInput(jobSrcFile);
	char line[MAX_LINE_LEN];
	
	while(jobInput.eof())
	{
		// Get Job info
		jobInput.getline(line, MAX_LINE_LEN);	// First line in job contains job info
		
		
		// Store Job instructions in an array
		
		
		// Get Data info
		// Store Data values in an array
		
		
		// Initialize new Process
		
		// Add Process to PCB
		
	}
	
	// Close job file
	jobInput.close()
	
}

void Loader::ParseJob(string line)
// Postcondition:	jobID, programSize, and priority set
{
	
}

void Loader::ParseData(string line)
// Postcondition:	buffer sizes set
{
	
}



int32_t ConvertHexToDec(string hex)
// Postcondition:	returns hex str converted to dec
{
	
}


int32_t ConvertHexCharToDec(char c)
// Precondition:	c is {0,1,2,3,4,5,6,7,8,9,A,B,C,D,E,F}
// Postcondition:	return char converted to decimal number
{
	
}
