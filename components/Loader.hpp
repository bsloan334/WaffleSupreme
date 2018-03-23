#pragma once

#include "PCBManager.hpp"
#include "Process.hpp"
#include "Disk.hpp"
#include "Types.hpp"

#include <vector>
#include <cstdlib>
#include <cstdint>
#include <cstring>
#include <string>
#include <queue>

using namespace std;

class Loader
{
    public:
        //TODO: Loader(Disk *disk)
		Loader(Disk* disk, PCBManager* pcb, queue<Process*>* newQueue);
        
        void LoadJobs(const string job_src_file);
        // Postcondition:    Jobs from src file have been loaded into memory
        //                    and stored as Process entries in the PCB
        instruction_t ConvertHexToDec(string hex);
        // Postcondition:    returns hex str converted to dec
    
    private:
        
        Disk* disk;
		PCBManager* pcb;
		queue<Process*>* newQueue;
        
        // Information extracted from Job section
        int jobID;
        size_t programSize;
        int priority;
        
        // Information extracted from Data section
        size_t inBufferSize;
        size_t outBufferSize;
        size_t tempBufferSize;
        
        // Information saved when process is stored in Disk [TEMP: RAM]
        int programBase;
        int inBufferBase;   // Derived
        int outBufferBase;  // Derived
        int tempBufferBase; // Derived
        
        void ParseJob(string line);
        // Postcondition:    jobID, programSize, and priority set
        
        void ParseData(string line);
        // Postcondition:    buffer sizes set
        
        void StoreJob(list<string>* fileData);
        // Postcondition:   fileData stored in Disk (RAM)
        
}; // end Loader
