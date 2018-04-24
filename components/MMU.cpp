#include "MMU.hpp"
#include <mutex>

using namespace std;

MMU::MMU(RAM* r, Disk* d) : ram(r), disk(d),
		frameBlock(d->Size() / PAGE_SIZE), pageBlock(r->Size() / PAGE_SIZE) {
	
	// Add all pages to the queue of free pages
	for (int i = 0; i < pageBlock.size(); i++)
		AddFreePage(i);

	// Set up the page table
	for (int i = 0; i < frameBlock.size(); i++)
		pageTable.push_back(Translation(i));

	PageFaultCount = 0;
}

/* REMOVING THIS AND JUST MOVING IT'S FUNCTIONALITY TO MMU CONSTRUCTOR
void MMU::SetFreeFrames() {
	for (int i = 0; i < (ram->Size() / (PAGE_SIZE)); i++) {
		AddFreeFrame(i);
	}
}
*/

b_address_t MMU::PageIndexToAddress(p_index_t page_i) {
	return page_i * PAGE_SIZE;
}

b_address_t MMU::FrameIndexToAddress(f_index_t frame_i){
	return frame_i * PAGE_SIZE;
}

//Writing a word of data to RAM
void MMU::WriteInstrToRAM(p_index_t page_i, i_size_t offset, instruction_t data)	// Write an instruction to RAM
// WARNING: Writes to RAM without checking mutex lock
{
	b_address_t address = PageIndexToAddress(page_i) + (offset * WORD);
	int act = ram->Allocate(data, address);
}

/* Write an entire frame to page in RAM (returns false for page fault)
  (Will spin until a free page is found) */
bool MMU::WriteFrameToRAM(f_index_t frame_i)
{
	bool pageFound = false;

	while (FreePages.size() > 0 && !pageFound)	// Spin until no more RAM or a page is found
	{
		p_index_t page_i = FreePages.front();
		if (pageBlock.pages[page_i].first.TestAndSet() == FREE)
		{	
			FreePages.pop_front();
			pageFound = true;
			pageTable[frame_i].page_i = page_i;
			for (int i = 0; i < INSTRS_PER_PAGE; i++)
			{	
				instruction_t data = disk->ReadInstruction(FrameIndexToAddress(frame_i) + WORD*i);
				WriteInstrToRAM(page_i, i, data);

			}
		}
	}

	return pageFound;
}


void MMU::ServicePageFault(f_index_t frame_i) // Puts frame_i into RAM
{
	WriteFrameToRAM(frame_i);
	while (PageFaultCountMutex.TestAndSet() == LOCK);	// Enter critical section
	PageFaultCount++;									// Critical section
	PageFaultCountMutex.ReleaseLock();					// Exit critical section
}

void MMU::Deallocate(Process* p)
// Sets all pages corresponding to it's frames to INVALID_PAGE
// Puts all its pages back at the end of FreePages
{
	for (b_address_t address = p->GetProgramBaseDisk();
		address < p->GetProgramEndDisk();
		address += PAGE_SIZE)
	{
		f_index_t frame_i = address / PAGE_SIZE;		// calculate frame index
		p_index_t page_i = pageTable[frame_i].page_i;	// save corresponding page index
		pageTable[frame_i].page_i = INVALID_PAGE;		// mark page index as empty (invalid)
		FreePages.push_back(page_i);					// put page index on queue of available frames
	}
}

void MMU::PreloadProcess(Process* p) {
	f_index_t frame_i;
	b_address_t diskAddress;
	bool instructionsAvailable = true;

	b_address_t programBase = p->GetProgramBaseDisk();
	b_address_t programEnd = p->GetProgramEndDisk();

	while (FreePages.size() == 0);	// Spin until some page is unused

	// Add first PRELOADED_PAGES pages into RAM
	for (int i = 0; instructionsAvailable && i < PRELOADED_PAGES; i++)
	{
		diskAddress = programBase + i * PAGE_SIZE;
		
		// Stop if there are no more instructions to load into RAM
		if (diskAddress >= programEnd)
			instructionsAvailable = false;
		
		// Other wise go ahead and load entire frame into RAM
		else
		{
			frame_i = diskAddress / (PAGE_SIZE);
			WriteFrameToRAM(frame_i);
		}
	}
}

//Translate Process's program counter to a physical address
instruction_t MMU::GetInstruction(b_address_t address) {
	f_index_t frame_i;
	i_address_t offset;
	bool pageServiced = false;

	instruction_t instr = NULL_ADDRESS;

	// Calculate frame index (for disk) and offset (for RAM)
	frame_i = address / PAGE_SIZE;
	offset = (address % PAGE_SIZE) / WORD;

	while (!pageServiced)
	{
		p_index_t page_i;

		/* Try to get the page from RAM */
		if ((page_i = pageTable[frame_i].page_i) != INVALID_PAGE){
			b_address_t ramAddress = PageIndexToAddress(page_i) + WORD * offset;
			instr = ram->GetInstruction(ramAddress);
			pageServiced = true;
			// STOP CLOCK here
		}

		/* Service a page fault */
		else {
			ServicePageFault(frame_i);
		}
	}

	return instr;
}

/*
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
	if (entry < (pageTable.size() - 1)) {
		pageTable.pages[entry].first = valid;
		pageTable.pages[entry].second = frame;
	}
}

void MMU::UpdatePageStack(b_size_t pageNumber) {
	auto contains = find(PageStack.begin(), PageStack.end(), pageNumber);
	if (contains != PageStack.end()) {
		PageStack.erase(contains);
	}
	PageStack.push_front(pageNumber);
	}
*/