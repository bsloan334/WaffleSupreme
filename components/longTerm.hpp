#pragma once
#include "Process.hpp"
#include "PCBManager.hpp"
#include "disk.hpp"
using namespace std;

class LongTerm {
   public:

      LongTerm();
      ~LongTerm();

      void ReadyToWait();  //Move process from ready to wait
      void WaitToReady(); //Move process from wait to ready
      void DiskToRam();  //Move processes from disk to ram

   private:
      struct BlankSpace {
         size_t instructionSize;
         size_t startAddress;
         BlankSpace(size_t start, size_t instructSize) :
               startAddress(start), instructionSize(instructSize) {}
      };

      struct UsedSpace {
         size_t begin;
         size_t end;
         UsedSpace(size_t beg, size_t en) : begin(beg), end(en) {}
      };

      size_t readyQueueSize;
      instruction_t maxAddress;
      //Might not work because it is wantint to use the "used" from disk.hpp not make a new one ****************
      vector<UsedSpace> used;
      vector<BlankSpace> FindBlankSpaces();

      bool IsBlank(BlankSpace blank, vector<UsedSpace> used);
      bool CheckResource(resourceType rType);
};