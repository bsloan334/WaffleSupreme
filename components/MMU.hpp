#pragma once
#include "RAM.hpp"
#include "Disk.hpp"
#include "Mutex.hpp"
#include "Cache.hpp"
#include <queue>
#include <vector>

#define PAGE_SIZE 4*4

using namespace std;

class MMU { 
   public:
      MMU();

      deque<b_size_t> PageStack;
      int PageFaultCount;
      long int PageFaultStartClock;
      long int PageFaultEndClock;
      long int PageFaultTimeClock;
      long int PageFaultServiceTime;
      //Use to keep track of last page used when a fault occurs
      b_size_t lastRequestedPage;

      b_size_t FreeFrameCount() {return FreeFrames.size();}
      b_size_t FrameNumberToIndex(b_size_t frame);
      b_size_t GetLastRequestedPage() { return lastRequestedPage; }
      b_size_t GetPageTableLength() { return pageTable.pages.size(); }
      b_size_t PopLRUPage() { b_size_t lru = PageStack.back(); PageStack.pop_back(); return lru; }
      pair<bool, b_size_t> GetPageTableEntry(b_size_t pageNumber);
      bool IsValidPage(b_size_t frame);
      bool ProcessDiskToRam(Process* p, b_size_t pageNumber);

      void SetFreeFrames();
      void AddFreeFrame(int index){FreeFrames.push(index);};
      void DumpProcess(PCBManager* p);
      void DumpPage(PCBManager* p);
      void WriteToRam(instruction_t index, instruction_t offset, instruction_t data);
      void SetPageTableEntry(b_size_t entry, bool valid, b_size_t frame);
      void UpdatePageStack(b_size_t pageNumber);
      void SetWaitForMMU(bool mmuWait) { waitForMMU = mmuWait; }
      void SetLastRequestedPage(b_size_t pageNumber) { lastRequestedPage = pageNumber; }
      void IncrementPageFaultCount() { PageFaultCount++; }
      void IncrementPageFaultServiceTime() { PageFaultServiceTime++; }
      void SetPageFaultStartClock() { PageFaultStartClock = clock_Tick; }
      void SetPageFaultEndClock() { 
         PageFaultEndClock = clock_Tick;
         PageFaultTimeClock += (PageFaultEndClock - PageFaultStartClock);
      }

      int GetPageFaultCount() { return PageFaultCount; }
      int GetPageFaultServiveTime() { return PageFaultServiceTime; }
      long int GetPageFaultTimeClock() { return PageFaultTimeClock; }

      instruction_t GetInstruction(PCBManager* p);
      instruction_t GetInstruction(PCBManager* p, instruction_t data);
      vector<instruction_t> GetFrameInfo(PCBManager p);

   private:
      queue<b_size_t> FreeFrames;
      Mutex mutex;
      b_size_t SwapPage;

      void WriteToDisk(PCBManager* p, b_size_t SwapPage);
      
      struct PageTable {
         vector<pair<bool, instruction_t>> pages;
         PageTable(b_size_t size) : pages(size) {
            for (auto target = pages.begin(); target != pages.end(); target++) {
               (*target).first = false;
               (*target).second = -1;
            }
         }
         b_size_t size() { return pages.size(); }
	   };

	   PageTable pageTable;
}