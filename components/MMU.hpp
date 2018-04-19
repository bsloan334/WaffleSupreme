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

      size_t FreeFrameCount() {return FreeFrames.size();}
      size_t FrameNumberToIndex(size_t frame);

      //LOcking setters
      void SetLock() {while (mutex == LOCK) {} mutex = LOCK;}
      void ReleaseLock() {mutex = FREE;}

      void SetFreeFrames();
      void AddFreeFrame(int index){FreeFrames.push(index);};
      void DumpProcess(PCBManager* p);
      void DumpPage(PCBManager* p);
      void WriteToRam(instruction_t index, instruction_t offset, instruction_t data);
      instruction_t GetInstruction(PCBManager* p);
      instruction_t GetInstruction(PCBManager* p, instruction_t data);
      vector<instruction_t> GetFrameInfo(PCBManager p);

   private:
      queue<size_t> FreeFrames;
      Mutex mutex;
      size_t SwapPage;

      void WriteToDisk(PCBManager* p, size_t SwapPage);
}