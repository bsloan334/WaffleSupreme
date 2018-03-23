#pragma once

#include "Process.hpp"
#include "Types.hpp"

#include <cstdlib>
#include <iostream>
#include <list>
#include <cstdint>

using namespace std;

class PCBManager
{ 
    public:
        PCBManager()
        {
            size = 0;
            listHead = NULL;
        }

        ~PCBManager()
        {

        }

        Process* FindProcess(int index);
        Process* GetPCBHead();
        int GetSize();
        void AddProcess(Process newProcess);

        void RemoveProcess(Process target);
        void SortPCB(int schedulingType);

    private:

        list<Process> PCB;

        int size;
        Process *listHead;
};

