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
   size_t DiskLocation = p->GetDiskAddress() + (pageNumber * (PAGE_SIZE));
   SetLock();
   
   for (int i = 0; i < ((PAGE_SIZE) / (4)); i++, DiskLocation += 4, address += 4) {
		disk.Allocate(DiskLocation, ram.GetInstruction(address));
	}
	ReleaseLock();
}

void DumpProcess(Process* p) {
   size_t frame;
   //implement GetPageTableLength, IsValidPage, GetFrame, SetPageTableEntry in pcb?
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
   // Still need to implement - SetLastRequestedPage, getpc, isvalid, getframe
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
		// Page Fault still need to implement. Not sure if tracking page fault here the right place
		//p->IncrementPageFaultCount();
		//p->SetPageFaultStartClock();
		return -1;
	}
}


vector<instruction_t> MMU::GetFrameInfo(Process* p) {
	int counter = p->GetProgramCounter();
	size_t offset = (p->GetProgramCounter() % (PAGE_SIZE));
	vector<instruction_t> instructions;
	instructions.resize(4); //size of 4 instructions in 1 frame
	for (unsigned int i = 0; i < 4; i++, counter +=4) {
		offset = (counter % (PAGE_SIZE)) / (4);
		instructions[offset] = (GetInstruction(p,counter));
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
		// Page Fault still need to implement. Not sure if tracking page fault is somethign we should do.
		//p->IncrementPageFaultCount();
		//p->SetPageFaultStartClock();
		return -1;
	}
}
