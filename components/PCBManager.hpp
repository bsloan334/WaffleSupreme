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
        }

        ~PCBManager()
        {
        }

		Process* FindProcess(int index);		/*Find process by index*/

		/*** Functions for traversing PCB ***/
		void Start();							/*Set PCB iterator to beginning of list*/
		void Next();							/*Progress process iterator*/
		bool AtEnd();							/*Returns whether or not itr is at end of list*/
		Process* CurrentProcess();				/*Return process at current position*/
        
		int GetSize();							/*Get number of process in PCB*/
        
		void AddProcess(Process* newProcess);	/*Add a process to the PCB*/

        void RemoveProcess(Process* target);
        void SortPCB(int schedulingType);

    private:
        list<Process*> PCB;
		list<Process*>::iterator itr;

};

