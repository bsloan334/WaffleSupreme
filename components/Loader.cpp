#include "Loader.hpp"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>

using namespace std;

Loader::Loader(Disk* disk_init)
{
    disk = disk_init;
}

void Loader::LoadJobs(string jobSrcFile, PCBManager* pcb)
// Postcondition:    Jobs from src file have been loaded into memory
//                    and stored as Process entries in the PCB
{

    // string to hold current data line
    string line;
    
    // Flags whether or not pogramBase is next instr to be inserted
    bool atProgramBase = false;
    
    // File reader for job file
    ifstream jobInput;
    
    jobInput.open(jobSrcFile.c_str(), ios::in);
    
    while (!jobInput.eof())
    {
        Process* process;
        
        // Get Job info
        getline(jobInput, line);    // First line in job contains job info
        
        cout << dec << "  line = " << line << endl;
        
        if (line.at(0) == '/')
		{
			if ( ( line.substr(0, 4) ).compare("// J") == 0 )
            {                          
                ParseJob(line);
                atProgramBase = true;       // indicate program head will be next instr inserted                
            }
            else if ( line.substr(0, 4).compare("// D") == 0)
            {
                ParseData(line);
            }
            else if ( line.substr(0, 4).compare("// E") == 0)
            {
                
                process = new Process(jobID, programSize, priority,
                                    inBufferSize, outBufferSize, tempBufferSize);
                
                inBufferBase = programBase + programSize;
                outBufferBase = inBufferBase + inBufferSize;
                tempBufferBase = outBufferBase + outBufferSize;
                
                process->SetProgramBase(programBase);
                process->AssignInputBase(inBufferBase);
                process->AssignOutputBase(outBufferBase);
                process->AssignTempBase(tempBufferBase);
                
                pcb->AddProcess(process);
                
            }
            else
            {
                ;// Indicate error
            }
            
        }
        else
        { 
            // Store instruction in disk
            int a = disk->Allocate(ConvertHexToDec(line));
            
            if(atProgramBase == true)
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
    tempString = new string( line.substr(i1, (i2-i1)) );
    jobID = ConvertHexToDec(*tempString);

    // Set programSize; 
    tempString = new string( line.substr(i2, (i3-i2)) );
    programSize = ConvertHexToDec(*tempString);
    
    // Set priority
    tempString = new string( line.substr(i3, (line.length()-i3)) );
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
    tempString = new string( line.substr(i1, (i2-i1)) );
    inBufferSize = ConvertHexToDec(*tempString);

    // Set output buffer size
    tempString = new string( line.substr(i2, (i3-i2)) );
    outBufferSize = ConvertHexToDec(*tempString);

    // Set temp buffer size
    tempString = new string( line.substr(i3, (line.length()-i3)) );
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
