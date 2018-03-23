#include "stdafx.h"
#include "ShortTerm.hpp"

using namespace std;

//Iniatilize the short term priority queue
ShortTerm::ShortTerm() {
   zeQueue = priority_queue<PCBManager*, deque<PCBManager*>>;
}

ShortTerm::~ShortTerm() {
   while(zeQueue.empty == false) {
      zeQueue.pop();
   }
}

PCBManager* ShortTerm::GetProcess() {
   return zeQueue.top();
}

void ShortTerm::AddProcess(PCBManager* process) {
   zeQueue.push(process);
}

void ShortTerm::RemoveProcess() {
   zeQueue.pop();
}

int ShortTerm:Size() {
   return zeQueue.Size();
}