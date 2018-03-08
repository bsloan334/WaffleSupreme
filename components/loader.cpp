#pragma once
#include "stdafx.h"
#include "PCBManager.hpp"
#include "constants.hpp"
#include "loader.hpp"

#include <stdint.h>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>

using namespace WaffleSupreme;


void Loader::LoadJobs(string jobSrcFile, PCBManager* pcb)
// Postcondition:	Jobs from src file have been loaded into memory
//					and stored as Process entries in the PCB
{
	// Initialize file reader and open file
	ifstream jobInput(jobSrcFile);

	// Char to hold current data line
	char line[MAX_LINE_LEN];

	// String array to hold file data
	string fileData[256];

	// Char used to check for lines with "//"
	char *checkTag[5];

	while (!jobInput.eof())
	{
		// Get Job info
		jobInput.getline(line, MAX_LINE_LEN);	// First line in job contains job info

		// Check for "//" tags and extract corresponding data
		memcpy(checkTag, &line[0], 5);
		if (checkTag[0] == "/")
		{
			if (strcmp(*checkTag, "// JOB"))
			{
				memcpy(line, &line[7], 6);
				ParseJob(line);
			}
			else if (strcmp(*checkTag, "// Data"))
			{
				memcpy(line, &line[8], 6);
				ParseData(line);
			}
			else
			{
				fileData->push_back(char("-1"));
			}
		}
		else
		{
			// Store instructions in an array
			fileData->push_back(*line);
		}
	}
	// Initialize new Process
	Process process(jobID, programSize, priority, inBufferSize, outBufferSize, tempBufferSize);

	// Add Process to PCB
	pcb->AddProcess(process);

	// Close job file
	jobInput.close();

}

void Loader::ParseJob(char* line)
// Postcondition:	jobID, programSize, and priority set
{
	string input(line,6);
	// Temporary hex string of JOB components
	string tempString;
	// Temporary integer to assign variables
	int tempInt;

	// Set jobID
	tempString = input.substr(0, 1);
	tempInt = ConvertHexToDec(tempString);
	jobID = tempInt;

	// Set programSize; 
	tempString = input.substr(2, 2);
	tempInt = ConvertHexToDec(tempString);
	programSize = tempInt;

	// Set priority
	tempString = input.substr(5, 1);
	tempInt = ConvertHexToDec(tempString);
	priority = tempInt;	
}

void Loader::ParseData(char* line)
// Postcondition:	input, output, and temp buffer sizes set
{
	string input(line, 6);
	// Temporary hex string of DATA components
	string tempString;
	// Temporary integer to assign variables
	int tempInt;

	// Set input buffer size
	tempString = input.substr(0, 2);
	tempInt = ConvertHexToDec(tempString);
	inBufferSize = tempInt;

	// Set output buffer size
	tempString = input.substr(4, 1);
	tempInt = ConvertHexToDec(tempString);
	outBufferSize = tempInt;

	// Set temp buffer size
	tempString = input.substr(5, 1);
	tempInt = ConvertHexToDec(tempString);
	tempBufferSize = tempInt;
}

int Loader::ConvertHexToDec(string hexString)
// Postcondition:	returns hex str converted to dec
{
	int dec;
	stringstream ss;
	ss << hex << hexString;
	ss >> dec;
	return dec;
}

//int Loader::ConvertHexCharToDec(char c)
// Precondition:	c is {0,1,2,3,4,5,6,7,8,9,A,B,C,D,E,F}
// Postcondition:	return char converted to decimal number
//{
	
//}
