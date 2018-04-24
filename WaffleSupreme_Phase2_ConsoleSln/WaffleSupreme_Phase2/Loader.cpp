#include "stdafx.h"
#include "Loader.h"
#include "Statistics.h"

#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>

using namespace std;

string GetFilePath(const string& fileName); // function gets path of current working dir and appends fileName

Loader::Loader(Disk* disk_init, PCBManager* pcb_init, queue<Process*>* newQueue_init, Statistics* stats_init)
{
	disk = disk_init;
	pcb = pcb_init;
	newQueue = newQueue_init;
	stats = stats_init;
}

void Loader::LoadJobs(string jobSrcFile)
// Postcondition:    Jobs from src file have been loaded into memory
//                    and stored as Process entries in the PCB
{
	statStruct *wait;			// Create new statStruct to begin saving waiting data
	string line;				// string to hold current data line
	bool atProgramBase = false;	// Flags whether or not pogramBase is next instr to be inserted
	ifstream jobInput;			// File reader for job file

								/*** Open Job file ***/
	jobInput.open(GetFilePath(jobSrcFile));

	if (!jobInput.is_open())
	{
		cout << "Could not open " << jobSrcFile << ". Exiting..." << endl;
		exit(EXIT_FAILURE);
	}

	/*** Parse Job File ***/
	while (!jobInput.eof())
	{
		Process* process;

		getline(jobInput, line);    // First line in job contains job info

		cout << dec << "  line = " << line << endl;

		/*** Case 1: Line is a section header. Parse header for process metadata ***/
		if (line.at(0) == '/')
		{
			if ((line.substr(0, 4)).compare("// J") == 0)	// JOB section header
			{
				ParseJob(line);
				atProgramBase = true;
			}
			else if (line.substr(0, 4).compare("// D") == 0)	// Data section header
			{
				ParseData(line);
			}
			else if (line.substr(0, 4).compare("// E") == 0 ||
						line.substr(0, 3).compare("//E") == 0)	// END tag indicating end of process
			{
				process = new Process(jobID, programSize, priority,
					inBufferSize, outBufferSize, tempBufferSize);
				process->SetProgramBase(programBase);
				pcb->AddProcess(process);

				stats->SetStats(0, jobID, std::chrono::system_clock::now(), std::chrono::system_clock::now(), true);

				newQueue->push(process);
			}
			else
			{
				;// Indicate error
			}

		}

		/*** Case 2: Line is an instruction. Insert into next available space in Disk ***/
		else
		{
			b_address_t a = disk->Allocate(ConvertHexToDec(line));
			if (atProgramBase == true)
			{
				atProgramBase = false;
				programBase = a;
			}

		}

	} // end while

	  // Close job file
	jobInput.close();


}

void Loader::ParseJob(string line)
// Postcondition:    jobID, programSize, and priority set
{
	// Temporary hex string of JOB components
	string* tempString;

	// Temporary integer to assign variables
	int i1, i2, i3;

	i1 = 3 + line.substr(3).find(" ") + 1;
	i2 = i1 + line.substr(i1).find(" ") + 1;
	i3 = i2 + line.substr(i2).find(" ") + 1;

	// Set jobID
	tempString = new string(line.substr(i1, (i2 - i1)));
	jobID = ConvertHexToDec(*tempString);

	// Set programSize; 
	tempString = new string(line.substr(i2, (i3 - i2)));
	programSize = ConvertHexToDec(*tempString);

	// Set priority
	tempString = new string(line.substr(i3, (line.length() - i3)));
	priority = ConvertHexToDec(*tempString);
}

void Loader::ParseData(string line)
// Postcondition:    input, output, and temp buffer sizes set
{
	// Temporary hex string of JOB components
	string* tempString;

	// Temporary integer to assign variables
	int i1, i2, i3;

	i1 = 3 + line.substr(3).find(" ") + 1;
	i2 = i1 + line.substr(i1).find(" ") + 1;
	i3 = i2 + line.substr(i2).find(" ") + 1;

	// Set input buffer size
	tempString = new string(line.substr(i1, (i2 - i1)));
	inBufferSize = ConvertHexToDec(*tempString);

	// Set output buffer size
	tempString = new string(line.substr(i2, (i3 - i2)));
	outBufferSize = ConvertHexToDec(*tempString);

	// Set temp buffer size
	tempString = new string(line.substr(i3, (line.length() - i3)));
	tempBufferSize = ConvertHexToDec(*tempString);
}

instruction_t Loader::ConvertHexToDec(const string hexString)
// Postcondition:    returns hex str converted to dec
{

	instruction_t instr;
	stringstream ss;
	ss << hex << hexString;
	ss >> instr;

	return instr;
}


/** function gets path of current working dir and appends fileName **/
string GetFilePath(const string& fileName) {
	string path = __FILE__; //gets source code path, include file name
	path = path.substr(0, 1 + path.find_last_of('\\')); //removes file name
	path += fileName; //adds input file to path
	return path;
}
