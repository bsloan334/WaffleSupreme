#pragma once
#include <cstdlib>
#include <string>
#include "PCB.hpp"
#include "Memory.hpp"
#include "ShortTermSched.hpp"

namespace WaffleSupreme
{
	
	class Loader
	{
		public:
			Loader();
			// Default constructor
			
			void LoadJobs(string job_src_file, Process* pcb_head);
			// Postcondition:	Jobs from src file have been loaded into memory
			//					and stored as Process entries in the PCB
			
		private:
			// Constant keys for scanning file
			const string JOB_START 	= "// JOB";
			const string DATA_START = "// DATA";
			const string JOB_END 	= "// END";
			
			// Maximum length of file line
			const size_t MAX_LINE_LEN = 17;
			
			// Information extracted from Job section
			int32_t jobID;
			size_t  programSize;
			int32_t priority;
			
			// Information extracted from Data section
			size_t inBufferSize;
			size_t outBufferSize;
			size_t tmpBufferSize;
			
			/** Functions to parse job and data info from string **/
			void ParseJob(string line);
			// Postcondition:	jobID, programSize, and priority set
			
			void ParseData(string line);
			// Postcondition:	buffer sizes set
			
			
			
	} // end Loader
	
	int32_t ConvertHexToDec(string hex);
	// Postcondition:	returns hex str converted to dec

	int32_t ConvertHexCharToDec(char c);
	// Precondition:	c is {0,1,2,3,4,5,6,7,8,9,A,B,C,D,E,F}
	// Postcondition:	return char converted to decimal number
	}
}
