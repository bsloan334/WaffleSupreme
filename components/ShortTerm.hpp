#pragma once
#include <queue>
#include "Process.hpp"

using namespace std;

class ShortTerm{
   public:
      

      priority_queue<Process*, deque<Process*>> zeQueue;

      ShortTerm();
      ~ShortTerm();

      Process* GetProcess();
      void AddProcess(Process* process);
      void RemoveProcess();
      int Size();
      bool IsEmpty() { return zeQueue.empty; };
};
