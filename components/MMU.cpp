#include "MMU.hpp"
#include <mutex>

extern RAM ram;
extern Disk disk

using namespace std;

MMU::MMU() {
   SetFreeFrames();
}

void MMU::SetFreeFrames() {
	for (int i = 0; i < (ram.size()/(PAGE_SIZE)); i++) {
		AddFreeFrame(i);
   }
}

size_t MMU::FrameNumberToIndex(size_t frame) { 
   return frame * PAGE_SIZE;
}

//Writing a word of data to RAM
void MMU::WriteToRam(instruction_t frame, instruction_t offset, instruction_t data) {
   instruction_t address = FrameNumberToIndex(frame);
   SetLock();
   ram.Allocate(address + offset, data);
   ReleaseLock();
}

void MMU::WriteToDisk(Process* p, size_t pageNumber) { 
   //Implement GetFrame in Process
   size_t frameNumber = p->GetFrame(pageNumber);
   size_t address = FrameNumberToIndex(frameNumber);
   //Eliminate any compile error from unused variables
   //Also look into how to get disk address
   size_t diskLocation = p->GetDiskAddress() + (pageNumber * (PAGE_SIZE));
   SetLock();
   
   for (int i = 0; i < ((PAGE_SIZE) / (4)); i++, diskLocation += 4, address += 4) {
		disk.Allocate(diskLocation, ram.GetInstruction(address));
	}
	ReleaseLock();
}

bool MMU::ProcessDiskToRam(Process* p, b_size_t pageNumber) {
   b_size_t diskLocation;
   b_size_t frameNumber;
   b_size_t frameSize;
   i_size_t address;

   frameSize = FreeFrames.size();
   if(frameSize == 0) {
      return false;
   }
   //Checking to see if we are trying to allocate more Ram than we have
   if (address + (PAGE_SIZE) > ram.size()) {
      return false;
   }

   diskLocation = p->GetDiskAddress() + (pageNumber * PAGE_SIZE);
   frameNumber = FreeFrames.front();
   address = FrameNUmberToIndex(frameNumber);

   //We can now be sure that the frame can be popped
   FreeFrames.pop();
   p->SetPageTableEntry(pageNumber, true, frameNumber);
   SetLock();
   //Allocate the frame to main memory now
   for(int i = 0; i < (PAGE_SIZE)/4; i++, diskLocation+=4, address+=4) {
      ram.Allocate(address, disk.ReadInstruction((diskLocation)));
   }
   ReleaseLock();
   return true;
}

void DumpProcess(Process* p) {
   size_t frame;
   for (int i = (p->GetPageTableLength() - 1); i > 0; i--) {
		if (p->IsValidPage(i)) {
			frame = p->GetFrame(i);
			WritePageToDisk(p, i);
			p->SetPageTableEntry(i, false, -1);
			FreeFrames.push(frame);
			break;
		}
	}
}

void DumpPage(Process* p) {
   size_t frame;
	for (int i = (p->GetPageTableLength() - 1); i > 0; i--) {
		if (p->IsValidPage(i)) {
			frame = p->GetFrame(i);
			WritePageToDisk(p, i);
			p->SetPageTableEntry(i, false, -1);
			FreeFrames.push(frame);
			break;
		}
	}
}

//Translate Process's program counter to a physical address
instruction_t MMU::GetInstruction(Process* p) {
	size_t frame;
	size_t offset;
   // Still need to implement - GetPC
	p->SetLastRequestedPage(p->GetProgramCounter() / (PAGE_SIZE));
	if (p->IsValidPage(p->GetProgramCounter()/(PAGE_SIZE))){
		frame = p->GetFrame(p->GetProgramCounter() / (PAGE_SIZE));
		offset = (p->GetProgramCounter() % (PAGE_SIZE));
		SetLock();
		instruction_t results = ram.GetInstruction((frame * (PAGE_SIZE)) + offset);
		ReleaseLock();
		return results;
	}
	else {
		p->IncrementPageFaultCount();
		p->SetPageFaultStartClock();
		return -1;
	}
}


vector<instruction_t> MMU::GetFrameInfo(Process* p) {
	int counter = p->GetProgramCounter();
	size_t offset = (p->GetProgramCounter() % (PAGE_SIZE));
	vector<instruction_t> instructions;
	instructions.resize(4); //size of 4 instructions in 1 frame
	for (int i = 0; i < 4; i++, counter +=4) {
		offset = (counter % (PAGE_SIZE)) / (4);
		instructions[offset] = (GetInstruction(p, counter));
	}
	return instructions;
}

//Translates a virtual address to a physical address, if one exists
instruction_t MMU::GetInstruction(Process* p, instruction_t address) {
	size_t pageNumber = (address / (PAGE_SIZE));
	p->SetLastRequestedPage(pageNumber);
		if (p->IsValidPage(pageNumber))
		{
			size_t frame = p->GetFrame(pageNumber);
			SetLock();
			instruction_t results = ram.GetInstruction((frame* (PAGE_SIZE)) + (address % (PAGE_SIZE)));
			ReleaseLock();
			return results;
		}
		else {
		p->IncrementPageFaultCount();
		p->SetPageFaultStartClock();
		return -1;
	}

   b_size_t MMU::GetFrame(b_size_t pageNumber) {
      return pageTable.pages[pageNumber].second;
   }
   
   pair<bool, b_size_t> MMU::GetPageTableEntry(b_size_t pageNumber) {
      return pageTable.pages[pageNumber];
   }

   bool MMU::IsValidPage(b_size_t pageNumber) {
      if (pageNumber > (pageTable.size() - 1)) {
         return false;
      } return pageTable.pages[pageNumber].first;
   }

   void MMU::SetPageTableEntry(b_size_t entry, bool valid, b_size_t frame) {
      if(entry < (pageTable.size() - 1)) {
         pageTable.pages[entry].first = valid;
         pageTable.pages[entry].second = frame;
      }
   }

   void MMU::UpdatePageStack(b_size_t pageNumber) {
      auto contains = find(PageStack.begin(), PageStack.end(), pageNumber);
	   if(contains != PageStack.end()) {
		   PageStack.erase(contains);
      }
	   PageStack.push_front(pageNumber);
   }
}
