#include "stdafx.h"
#include "LongTerm.hpp"

using namespace std;

LongTerm::LongTerm() { 
   readyQueueSize = 0;
   maxAddress = 0;
}

LongTerm::~LongTerm() {

}

void LongTerm::DiskToRam() {
   //vector to temp addresses of blank spaces
   vector<BlankSpace> blankSpaceLocations = FindBlankSpaces(); 
   //Variable describing all the times a process doesnt fit
   int noFitFound = 0; 
   if(readyQueueSize < DEAFULT_RAM) {
      while(!newQueue.empty()) {
         //Assign a NEW process into a space
         if(newQueue.GetProcess()->CheckState() == NEW) {
            noFitFound++;
            //seeing if the process fits in any space
            for(int i = 0; i < blankSpaceLocations.size(); i++) {
               if((blankSpaceLocations[i].instructionSize - blankSpaceLocations[i].begin) >= (newQueue.GetProcess()->GetEndAddress())) {
                  readyQueueSize += (newQueue.GetProcess()->GetEndAddress());
                  //If ram is full then dont allocate space
                  if(readyQueueSize >= DEAFULT_RAM) {
                     break;
                  }
                  lock_guard<mutex_> lock(mtx);
                  //STILL NEED TO MAKE FUNCTIONS: GetDiskAddress() and SetRamAddress() ************************
                  //IT IS REFERENCED BELOW
                  //PROBABLY IN THE PCB  OR DISK
                  RAM.AllocateChunk(blankSpaceLocations[i].begin, Disk.ReadInstructionChunk(newQueue.GetProcess()->GetDiskAddress(), newQueue.GetProcess()->GetEndAddress()));
                  cout << ("Process allocated to RAM with the Process ID of " + newQueue.GetProcess()->GetID());
                  used.push_back(Used(blankSpaceLocations.begin, blankSpaceLocations.instructionSize));
                  //SET RAM ADDRESS used here using setprogrambase for now **************
						newQueue.GetProcess()->SetProgramBase(blankSpaceLocations.begin);
						blankSpaceLocations.begin = blankSpaceLocations.begin + newQueue.GetProcess()->GetEndAddress();
						//Add to ready queue and update PCB
						newQueue.GetProcess()->SetState(READY);
						readyQueue.AddProcess(newQueue.GetProcess());
						newQueue.RemoveProcess();
						noFitFound--;
						break;
               }
            }
            
            //Look for a process that can fit in the new space
            if (readyQueueSize >= DEFAULT_RAM) {
					readyQueueSize = readyQueueSize - (newQueue.GetProcess()->GetEndAddress());
					noFitFound--;
					break;
				}
            //Place process in ready state at the end of the max end address
				else if (noFitFound > 0) {
					noFitFound = 0;
					readyQueueSize += (newQueue.GetProcess()->GetEndAddress());
					if (readyQueueSize < DEFAULT_RAM) {
						if (CheckEmpty(EmptySpace(maxAddress, maxAddress + newQueue.GetProcess()->GetEndAddress()), used)) {
                     //SET RAM ADDRESS IS USED HERE AS WELL ************* Dont know is set program base works
							newQueue.GetProcess()->SetProgramBase(maxAddress);
							lock_guard<mutex> lock(mtx);
                     //get disk address is referenced here ***********
							RAM.AllocateChunk(maxAddress, Disk.ReadInstructionChunk(newQueue.GetProcess()->GetDiskAddress(), newQueue.GetProcess()->GetEndAddress()));
							used.push_back(Used(maxAddress, maxAddress + newQueue.GetProcess()->GetEndAddress()));
							maxAddress = maxAddress + (newQueue.GetProcess()->GetEndAddress());

							//add to ready queue and update pcb
							cout << ("Process allocated to RAM with the Process ID of " + newQueue.GetProcess()->GetID());
							newQueue.GetProcess()->SetState(READY);
							readyQueue.AddProcess(newQueue.GetProcess());
							newQueue.RemoveProcess();
							if (readyQueueSize == DEFAULT_RAM) {
								readyQueueSize = readyQueueSize - ((newQueue.GetProcess()->GetEndAddress()));
								break;
							}
						}
						else {
							readyQueueSize = readyQueueSize - ((newQueue.GetProcess()->GetEndAddress()));
							break;
						}
					}
					else {
						readyQueueSize = readyQueueSize - ((newQueue.GetProcess()->GetEndAddress()));
						break;
					}

				}
         }
      }
   }
}
/*
ReadyToWait() and WaitToReady() operate off something that has not been implemeneted yet.
We need a way to check what type of resource is being used by a process, so that if a process is waiting 
on that resource, we can switch the process to waiting.
I have implemented CheckResource() but not GetResourceStatus(). I am not sure if something like this is 
implemented somewhere else or not. If not, it would probably be a good idea to put this is the PCB
An enum would be nice and should look something like this:
enum resourceType {
	NONE,
   KEYBOARD,
	DISK_IO,
	SHAREDMEMORY,
   STDOUT
};
I don't know if we will need all these resoruces, but at the very least, I need NONE for these functions.

UPDATE: I HAVE PUT IT IN PCBMANGER. FEEL FREE TO CHANGE IT IF IT SHOULD GO ELSEWHERE
*/
void LongTerm::ReadyToWait() {
	
	vector<PCBManager*> temp;

	while (readyQueue.size() > 0) {
		if (readyQueue.GetProcess()->GetResourceStatus() != resourceType::NONE) {
			if (CheckResource( readyQueue.GetProcess()->GetResourceStatus() ) == false ) {
				waitingQueue.AddProcess(readyQueue.GetProcess());
				readyQueue.GetProcess()->set_status(WAITING);
				readyQueue.RemoveProcess();
			}
		}
			temp.push_back(readyQueue.GetProcess());
			readyQueue.RemoveProcess();

	}
   //Fill the ready queue back up
	while (temp.empty()==false) {
		readyQueue.AddProcess(temp.back());
		temp.pop_back();
	}
}

void LongTerm::WaitToReady() {

	vector<PCBManager*> temp;

	while (waitingQueue.size() > 0) {
		if (waitingQueue.GetProcess()->GetResourceStatus() != resourceType::NONE) {
			if (CheckResource(waitingQueue.GetProcess()->GetResourceStatus()) == true) {
				readyQueue.AddProcess(waitingQueue.GetProcess());
				waitingQueue.GetProcess()->set_status(READY);
				waitingQueue.RemoveProcess();
			}
		}
		temp.push_back(waitingQueue.GetProcess());
		waitingQueue.RemoveProcess();
	}
   //Fill the waiting queue back up
	while (temp.empty() == false) {
		waitingQueue.AddProcess(temp.back());
		temp.pop_back();
	}
}

//Function to see if an certain section of RAM is empty
bool LongTerm::IsBlank(BlankSpace blank, vector<UsedSpace> used) {
   bool isEmpty = true;

   for(int i = 0; i < used.size(); i++){
      if(blank.startAddress >= used[i].begin && blank.instructionSize <= used[i].end) {
         isEmpty = false;
      } else if(blank.startAddress <= used[i].begin && blank.instructionSize > used[i].begin) {
         isEmpty = false;
      }
   }
   return isEmpty;
}

//Function to return a vecotr of all the blank spaces
vector<LongTerm::BlankSpace> LongTerm::FindBlankSpaces() {
   vector<BlankSpace> blankSpace;
   used.clear();
   readyQueueSize = 0;
   maxAddress = 0;
   int blankEnd = 0;

/*
processList probably needs to be changed. All it needs to be is a list of processes(it might be able to be pointers to them),
and the function needs to be able to iterate throught them to check the state. *****************************************
*/

   if(readyQueue.size() > 0) {
      //Go through the list of process and see which one is ready
      for(int i = 0; i < processList.size(); i++) {
         //Checks to see if the process has a status of READY
         if(processList[i].CheckState() == READY){
            used.push_back(UsedSpace(processList[i].GetProgramBase(), processList[i].GetProgramBase() + (processList[i].GetEndAddress())));
            readyQueueSize += (processList[i].GetEndAddress());
            maxAddress = readyQueueSize;
         }
      }

      if(used.size() > 0) {
         sort(used.begin, used.end());
         blankEnd = 0;

         for(int i = 0; i < used.size(); i++) {
            //look at the next process and see if there is a hole
            if(blankEnd < used.[i].begin) {
               //Check to see if it is empty and stores the beginning and end address if so
               if(IsBlank(BlankSpace(blankEnd, used[i].begin), used) {
                  blankSpace.push_back(BlankSpace(blankEnd, used[i].begin));
               }
            }
            blankEnd = used[i].end;
         }
      }
   }
   return blankSpace;
}

//Checks to see if another process that requires the same resource is in the running state
/* 
I USE PROCESS LIST AND GET RESOURCE STATUS HERE, SO WHEN THEY ARE IMPLEMENTED ELESWHERE,
PLEASE UPDATE THEM HERE TOO. *****************************************
*/
bool LongtTerm::CheckResource(resourceaType rType) {
   bool deadlock = true;

   for(int i = 0; i <processList.size(); i++) {
      if(processList[i].GetState() == RUNNING) {
         if(processList[i].GetResourceStatus() == rType) {
            deadlock = false;
         }
      }
   }
   return deadlock;
}
