#pragma once
#include <queue>
#include "PCBManager.hpp"

using namespace std;

class ShortTerm{
   public:
      

      priority_queue<PCBManager*, deque<PCBManager*>> zeQueue;

      ShortTerm();
      ~ShortTerm();

      PCBManager* GetProcess();
      void AddProcess(PCBManager* process);
      void RemoveProcess();
      int Size();
      bool IsEmpty() { return zeQueue.empty; };

};