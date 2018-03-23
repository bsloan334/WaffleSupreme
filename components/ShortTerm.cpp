#pragma once

#include "ShortTerm.hpp"

using namespace std;

//Iniatilize the short term priority queue
ShortTerm::ShortTerm() {
}

ShortTerm::~ShortTerm() {
   while(zeQueue.empty == false) {
      zeQueue.pop();
   }
}

Process* ShortTerm::GetProcess() {
   return zeQueue.top();
}

void ShortTerm::AddProcess(Process* process) {
   zeQueue.push(process);
}

void ShortTerm::RemoveProcess() {
   zeQueue.pop();
}

int ShortTerm::Size() {
   return zeQueue.size();
}
