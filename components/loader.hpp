#pragma once
#include "PCBManager.hpp"
#include "ShortTerm.hpp"
#include "ram.hpp"

#include <vector>
#include <cstdlib>
#include <cstdint>
#include <string>

namespace WaffleSupreme
{
	
	class Loader
	{
		public:
			Loader()
			{

			}
			// Default constructor
			
			void LoadJobs(string job_src_file, PCBManager* pcb);
			// Postcondition:	Jobs from src file have been loaded into memory
			//					and stored as Process entries in the PCB
			int ConvertHexToDec(string hex);
			// Postcondition:	returns hex str converted to dec
			int ConvertHexCharToDec(char c);
			// Precondition:	c is {0,1,2,3,4,5,6,7,8,9,A,B,C,D,E,F}
			// Postcondition:	return char converted to decimal number
			
		private:
			// Dynamic array to hold job file data
			vector<string> dataFileArray;
			
			// Information extracted from Job section
			int jobID;
			size_t  programSize;
			int priority;
			
			// Information extracted from Data section
			size_t inBufferSize;
			size_t outBufferSize;
			size_t tempBufferSize;
			
			/** Functions to parse job and data info from string **/
			void ParseJob(char* line);
			// Postcondition:	jobID, programSize, and priority set
			
			void ParseData(char* line);
			// Postcondition:	buffer sizes set
			
			
			
	}; // end Loader
};

//int ConvertHexToDec(string hex);
// Postcondition:	returns hex str converted to dec
//int ConvertHexCharToDec(char c);
// Precondition:	c is {0,1,2,3,4,5,6,7,8,9,A,B,C,D,E,F}
// Postcondition:	return char converted to decimal number
