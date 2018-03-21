#pragma once

#include "Process.hpp"
#include "Types.hpp"

#include <stdlib.h>
#include <iostream>
#include <list>
#include <stdint.h>

using namespace std;

class PCBManager
{ 
    public:
        PCBManager()
        {
            size = new int(0);
            listHead = NULL;
        }

        ~PCBManager()
        {

        }

        Process* GetPCBHead();
        void AddProcess(Process *newProcess);

        //void RemoveProcess(Process target);
        //void SortPCB(int schedulingType);

    private:

        list<Process> PCB;

        int *size;
        Process* listHead;
};

