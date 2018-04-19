#pragma once

#include "Process.hpp"
#include "PCBManager.hpp"
#include "Disk.hpp"
#include "RAM.hpp"
#include "MMU.hpp"

#include <queue>

using namespace std;

enum Scheduler { FIFO, PRIORITY };

class LongTerm {
   public:

	   LongTerm(queue<Process*>* newQ,
			    queue<Process*>* zeQ,
				RAM* r, Disk* d, PCBManager* p,
				int t) :
				newQueue(newQ), zeQueue(zeQ),
				ram(r), disk(d), p(p), scheduleType(t)
	   {};
      ~LongTerm();

      void ReadyToWait(); //Move process from ready to wait  <-Do we need to do this?
      void WaitToReady(); //Move process from wait to ready  

	  // Interface for ShortTerm
	  bool FillZeQueue();
	  // Puts as many processes as possible into RAM
	  //	Takes them off the newQueue and puts them onto zeQueue

	  /*
	  THIS IS WHERE PAGING STARTS
	  */

		size_t FrameSize();

		void LoadProcess(Process* p, size_t pageNumber);
		bool LoadPage(Process* p, size_t pageNumber);
		void DumpProcess(Process* p);
		void DumpFrame(Process* p);
		int InitialLoad();

   private:
      struct BlankSpace {
         i_size_t instructionSize;
         b_address_t startAddress;
         BlankSpace(b_address_t start, i_size_t instructSize) :
               startAddress(start), instructionSize(instructSize) {}
      };

      struct UsedSpace {
         b_address_t begin;
         b_address_t end;
         UsedSpace(b_address_t beg, b_address_t en) : begin(beg), end(en) {}
      };

	  // Params passed in from Driver
	  queue<Process*>* newQueue;
	  queue<Process*>* zeQueue;
	  RAM* ram;
	  Disk* disk;
	  PCBManager* p;
	  int scheduleType;

      size_t readyQueueSize = 0;
      instruction_t maxAddress = 0;
      //Might not work because it is wantint to use the "used" from disk.hpp not make a new one ****************
      vector<UsedSpace> used;
      vector<BlankSpace> FindBlankSpaces();

	  Process* GetNextProcess();

      bool IsBlank(BlankSpace blank);
      //bool CheckResource(resourceType rType);



	  void printQ()
	  {
		  queue<Process*> tmp(*zeQueue);
		  cout << "Printing zeQueue...";
		  while (!tmp.empty())
		  {
			  cout << " pid: " << tmp.front()->GetID();
			  tmp.pop();
		  }
		  cout << endl;

	  }
};