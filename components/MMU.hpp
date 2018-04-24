#pragma once
#include <sstream>
#include <iostream>
#include <vector>
#include <chrono>
#include <string>

#include "RAM.hpp"
#include "Disk.hpp"
#include "Mutex.hpp"
#include "Cache.hpp"
#include "Types.hpp"
#include "Process.hpp"

// Number of Pages per process
#define PRELOADED_PAGES 4

// Number of instructions in a page (or frame)
#define INSTRS_PER_PAGE 4

// Number of bytes in a page
#define PAGE_SIZE 16

// Indicates invalid page
#define INVALID_PAGE -1

using namespace std;

class MMU {
public:
	MMU(RAM* r, Disk* d);

	/********** Interface ***********************************************************************/
	bool WriteFrameToRAM(f_index_t frame_i);			// Wrtie an entire frame to page in RAM (returns false if there is no room in RAM)
	void PreloadProcess(Process* p);					// Preload pages
	void Deallocate(Process* p);						// Deallocate pages
	instruction_t GetInstruction(b_address_t address);	// Replaces RAM::GetInstruction(b_address_t) for paging

	friend ostream& operator << (ostream& o, MMU& mmu) { o << mmu.out; }

private:

	/********** Page table to look up RAM-Disk Translation **************************************/
	// first = frame_i, second = page_i
	struct Translation {
		int frame_i;
		int page_i;
		Translation(int f, int p = INVALID_PAGE) : frame_i(f), page_i(p){}
	};
	vector<Translation> pageTable;

	/********** Structs for Frames (Disk) and Pages (RAM) ***************************************/
	struct Frame {		// Frames for Disk (IDs are their indices in Frames vector)	
		i_address_t addressDisk;
		Frame(i_address_t a = -1) : addressDisk(a){};
	};

	struct Page {		// Pages for RAM (IDs are their indices in Pages vector)
		i_address_t addressRAM;
		Page(i_address_t a = INVALID_PAGE) : addressRAM(a){};
	};

	/********* Pages to "overlay" RAM ***********************************************************/
	struct Pages {
		vector<pair<Mutex, Page>> pages;
		Pages(p_size_t size) : pages(size) {
			for (auto target = pages.begin(); target != pages.end(); target++) {
				(*target).first = Mutex();
				(*target).second = NULL;
			}
		}
		p_size_t size() { return pages.size(); }
	};

	/********** Frames to "overlay" Disk ********************************************************/
	struct Frames {
		vector<pair<Mutex, Frame>> frames;
		Frames(f_size_t size) : frames(size) {
			for (auto target = frames.begin(); target != frames.end(); target++) {
				(*target).first = Mutex();
				(*target).second = NULL;
			}
		}
		f_size_t size() { return frames.size(); }
	};


	// Page operations (RAM)
	void ServicePageFault(f_index_t frame_i);										// Puts frame_i into RAM
	void WriteInstrToRAM(p_index_t page_i, i_size_t offset, instruction_t data);	// Write an instruction to RAM
	p_size_t FreePageCount() { return FreePages.size(); }					// Returns number of available pages in RAM
	b_address_t PageIndexToAddress(p_size_t page_i);						// Converts a page index to an instruction address in RAM
	b_address_t FrameIndexToAddress(f_size_t frame_i);
	
	void AddFreePage(p_index_t index){ FreePages.push_back(index); };		// Adds the index of a free page to the queue

	int GetPageFaultCount() { return PageFaultCount; }


	/********** MMU Data Structures ***************************************************/

	RAM* ram;
	Disk* disk;

	deque<p_index_t> FreePages;		// Stores queue of free page indices
									//	in a cycle-through fashion (push to end, pull from beginning)

	b_size_t SwapPage;

	int PageFaultCount;
	Mutex PageFaultCountMutex;

	long int PageFaultStartClock;
	long int PageFaultEndClock;
	long int PageFaultTimeClock;
	long int PageFaultServiceTime;
	//Use to keep track of last page used when a fault occurs
	b_size_t lastRequestedPage;

	void WriteToDisk(Process* p, b_size_t SwapPage);

	/******* Frames and Pages ***********************************************************/
	Frames frameBlock;
	Pages pageBlock;

	stringstream out;
};