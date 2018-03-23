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

}

void LongTerm::ReadyToWait() {

}

void LongTerm::WaitToReady() {

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

   if(readyQueue.size() > 0) {
      //Go through the list of process and see which one is ready
      for(int i = 0; i < processList.size(); i++) {
         //Checks to see if the process has a status of READY
         if(processList[i].CheckState() == READY){
            used.push_back(UsedSpace(processList[i].GetRamAddress(), processList[i].GetRamAddress() + (processList[i].GetEndAddress())));
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
/*
still need to implement:
processList
readyQueue
GetStatus
*/