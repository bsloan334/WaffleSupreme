#include "LongTerm.hpp"

using namespace std;

extern mmu mmu;

LongTerm::~LongTerm() {

}

/*** Move first Process p in zeQueue from Disk to RAM ***/
bool LongTerm::FillZeQueue() {

/*   //vector to temp addresses of blank spaces
   vector<BlankSpace> blankSpaceLocations = FindBlankSpaces(); 
   //Variable describing all the times a process doesnt fit
   int noFitFound = 0; 
   if(readyQueueSize < DEFAULT_RAM) {
*/
    
	// Return false and do nothing if no new processes are waiting on the new queue
	if (newQueue->empty())
		return false;

	bool ramFull = false;
	b_address_t ramProgramBase;
	Process* p;

	while (!ramFull && !newQueue->empty())
	{
		p = newQueue->front();
		//p = GetNextProcess();

		/*
			  //Assign a NEW process into a space
			  if(p->CheckState() == NEW) {
			  noFitFound++;
			  //seeing if the process fits in any space
			  for(int i = 0; i < blankSpaceLocations.size(); i++) {

			  int ramAddressStart = blankSpaceLocations[i].startAddress;

			  if((blankSpaceLocations[i].instructionSize - ramAddressStart)
			  >= (p->GetProgramEnd())) {
			  readyQueueSize += (p->GetProgramEnd());
			  //If ram is full then dont allocate space
			  if(readyQueueSize >= DEFAULT_RAM) {
			  break;
			  }
			  //lock_guard<mutex_> lock(mtx);
			  //STILL NEED TO MAKE FUNCTIONS: GetDiskAddress() and SetRamAddress() ************************
			  //IT IS REFERENCED BELOW
			  //PROBABLY IN THE Process  OR DISK
			  int diskAddressStart = p->GetProgramBase();
			  int diskAddressEnd = p->GetProgramEnd();

			  */
		queue<instruction_t> instrs = disk->ReadInstructionChunk(p->GetProgramBase(), p->GetProgramSize());
		ramProgramBase = ram->AllocateChunk(&instrs, p->GetID());

		if (ramProgramBase >= 0)
		{
			newQueue->pop();	// Take process off newQeue and push onto zeQueue
			zeQueue->push(p);
			p->SetState(READY);
			p->SetProgramBase(ramProgramBase);
		}
		else
			ramFull = true;
	}

	return true;
}

/*
PAGING FOR PHASE 2 STARTS HERE
*/

Mutex mutex;
void SetLock() { 
	while (mutex == LOCK) {
		cout << ("Frame is locked");
	}
	mutex = LOCK; 
}
void ReleaseLock() { mutex = FREE; }

b_size_t LongTerm::FrameSize() {
	SetLock();
	b_size_t FreeFrames = mmu.FreeFrameCount();
	ReleaseLock();
	return FreeFrames;
}

void LongTerm::LoadProcess(Process* p, b_size_t pageNumber) {
	// Load 4 pages into RAM
	for (int i = pageNumber; i < (pageNumber + 4); ++i) {
		if (mmu.ProcessDiskToRam(p, i) ) {    //Implement ProcessDiskToRam in mmu
			continue;
		}
		else {
			cout << ("No frames are available");
			return;
		}
	}

	p->SetState(READY);
	ReadyQueue.AddProcess(p);
}

void LongTerm::DumpProcess(Process* p) {
	SetLock();
	mmu.DumpProcess(p);
	ReleaseLock();
}

void LongTerm::DumpFrame(Process* p) {
	SetLock();
	mmu.DumpPage(p);
	ReleaseLock();
}

int LongTerm::InitialLoad() {	
	int numberOfJobs = 0;
	while (!newQueue.empty()) {
		LoadProcess(newQueue.GetProcess(), 0);
		numberOfJobs++;
		newQueue.removeProcess();
	}
	return numberofjobs;
}

/*
Process* LongTerm::GetNextProcess()
// Marches through the Pcb looking for Processes marked 'NEW'
//		and adds either the Job with the lowest Job ID or the highest Priority number to ZeQueue
{
	Process* p = NULL;
	bool newProcessFound = false;

	switch (scheduleType)
	{
		case FIFO:
			for (int i = 0; p != NULL && i < p->GetSize(); i++)
				if (p->FindProcess(i)->CheckState() == NEW)
					p = p->FindProcess(i);
			break;

		case PRIORITY:
			if (p->GetSize() > 0)
			{
				Process* pNext = p->FindProcess(0);
				for (int i = 1; i < p->GetSize(); i++)
					if (p->FindProcess(i)->GetPriority() > pNext->GetPriority())
						pNext = p->FindProcess(i);
				
			}
			break;

		default:
			cout << "OOPS! Invalid scheduling type passed as param to LongTerm.";
	}

	return p;
}
*/

	/*
	blankSpaceLocations[i].startAddress += p->GetProgramEnd(); 
				  //Add to ready queue and update Process
				  p->SetState(READY);
				  zeQueue->push(p);
				  newQueue->pop();
				  noFitFound--;
				  break;
               }
            }
            
            //Look for a process that can fit in the new space
            if (readyQueueSize >= DEFAULT_RAM) {
					readyQueueSize = readyQueueSize - p->GetProgramEnd();
					noFitFound--;
					break;
				}
            //Place process in ready state at the end of the max end address
				else if (noFitFound > 0) {
					noFitFound = 0;
					readyQueueSize += (p->GetProgramEnd());
					if (readyQueueSize < DEFAULT_RAM) {
						if (IsBlank(BlankSpace(maxAddress, maxAddress + p->GetProgramEnd()))) {
                     //SET RAM ADDRESS IS USED HERE AS WELL ************* Dont know is set program base works
							p->SetProgramBase(maxAddress);

							//lock_guard<mutex> lock(mtx);
                     //get disk address is referenced here ***********

							ram->AllocateChunk(maxAddress, disk->ReadInstructionChunk(p->GetDiskAddress(), p->GetProgramEnd()));
							used.push_back(UsedSpace(maxAddress, maxAddress + p->GetProgramEnd());
							maxAddress = maxAddress + (p->GetProgramEnd());

							//add to ready queue and update p
							cout << ("Process allocated to RAM with the Process ID of " + p->GetID());
							p->SetState(READY);
							zeQueue->push(newQueue->front());
							newQueue->pop();
							if (readyQueueSize == DEFAULT_RAM) {
								readyQueueSize = readyQueueSize - ((p->GetProgramEnd()));
								break;
							}
						}
						else {
							readyQueueSize = readyQueueSize - ((p->GetProgramEnd()));
							break;
						}
					}
					else {
						readyQueueSize = readyQueueSize - ((p->GetProgramEnd()));
						break;
					}

				}
			}
	  }
   }*/

/*
ReadyToWait() and WaitToReady() operate off something that has not been implemeneted yet.
We need a way to check what type of resource is being used by a process, so that if a process is waiting 
on that resource, we can switch the process to waiting.
I have implemented CheckResource() but not GetResourceStatus(). I am not sure if something like this is 
implemented somewhere else or not. If not, it would probably be a good idea to put this is the Process
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
/*
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
} */



/* ========================================================================================= */
/* I moved the blank space finding to RAM so that LongTerm doesn't have to deal with this and
   managing process states at the same time. RAM remembers every location memory is allocated
   and "forgets" every location that is deallocated, meaning we will now have to manually
   deallocate every TERMINATED process. There are no safeguards for using the RAM::Allocate
   function, but RAM::AllocateChunk will return false if there is insufficient RAM to hold
   the process.
   - Ruth
 */
/* ========================================================================================= */

/*
//Function to see if an certain section of RAM is empty
bool LongTerm::IsBlank(BlankSpace blank) {
	bool isEmpty = true;

	for (int i = 0; i < used.size(); i++){
		if (blank.startAddress >= used[i].begin && blank.instructionSize <= used[i].end) {
			isEmpty = false;
		}
		else if (blank.startAddress <= used[i].begin && blank.instructionSize > used[i].begin) {
			isEmpty = false;
		}
	}
	return isEmpty;
}
*/

//Function to return a vecotr of all the blank spaces
// (key RUNNING indicates process is in RAM; NEW, READY, WAITING, TERMINATED indicates they are not)
/*** THIS IS NOW HANDLED IN RAM ******************************************************************
vector<LongTerm::BlankSpace> LongTerm::FindBlankSpaces() {
   vector<BlankSpace> blankSpace;
   used.clear();
   readyQueueSize = 0;
   maxAddress = 0;
   int blankEnd = 0;

   if(zeQueue->size() > 0) {
      //Go through the list of process and see which one is ready
      for(int i = 0; i < p->GetSize(); i++) {

		  Process* p = p->FindProcess(i);

         //Checks to see if the process has a status of RUNNING, indicating it is in RAM
         if(p->CheckState() == RUNNING){
            used.push_back(UsedSpace(p->GetProgramBase(), p->GetProgramBase() + p->GetProgramEnd()));
            
			// try: maxAddress += p->GetProgramEnd();

			readyQueueSize += p->GetProgramEnd();
            maxAddress = readyQueueSize;
         }
      }

      if(used.size() > 0) {
         sort(used.begin(), used.end());

         blankEnd = 0;

         for(int i = 0; i < used.size(); i++) {
            //look at the next process and see if there is a hole
            if(blankEnd < used[i].begin) {
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
*******************************************************/


/* ===================================================================================== */
/* Marking this as Phase 2 - Ruth */
/* ===================================================================================== */
//Checks to see if another process that requires the same resource is in the running state
/* 
I USE PROCESS LIST AND GET RESOURCE STATUS HERE, SO WHEN THEY ARE IMPLEMENTED ELESWHERE,
PLEASE UPDATE THEM HERE TOO. *****************************************

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
****/