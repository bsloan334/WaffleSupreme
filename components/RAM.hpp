#pragma once

#include <vector>
#include <iostream>
#include <queue>
#include <list>

#include "Types.hpp"

//This makes the ram byte-addressable
#define DEFAULT_RAM 1024 * WORD

enum Used{ FREE,	// Does not contain active process instruction
		   FLAGGED	// Process has flagged section in order to write to it {DANGER: This needs to be protected with mutex}
};

using namespace std;

class RAM {

public:

	struct Section {	// A section of memory marked either as used or free (addressed as instruction)
		i_address_t first;
		i_address_t last;
		bool status;
		int pid;			// (only set if status = FLAGGED)
	};

	RAM(size_t size = DEFAULT_RAM) : size(size), storage(size, 0) {
		Section* s = new Section();
		s->first = 0;
		s->last = size / WORD;
		s->status = FREE;

		blankSpaces.push_back(s);
	}

	b_address_t Allocate(byte_t data, b_address_t index);
	b_address_t Allocate(instruction_t data, b_address_t index);
	b_address_t AllocateChunk(queue<instruction_t>* instructions, int pid); // WARNING: This will "spin" until there is room in RAM to put it

	void Deallocate(b_address_t start, b_size_t length);	// dealloc instructions starting at index
	// startIndex and length are both byte addresses (not instruction address)

	b_size_t Size() { return size; }
	instruction_t GetInstruction(b_address_t index);
	string GetStatus();

	Section* FirstAvailableSection(size_t instrNbr);


	void printAvailableSpace()
	{
		b_size_t totalRamUsage = DEFAULT_RAM;
		double totalRam = DEFAULT_RAM;

		cout << "RAM usage stats..." << endl;
		for ( list<Section*>::iterator itr = blankSpaces.begin();
			  itr != blankSpaces.end();
			  itr++)
		{
			cout << dec << "  blank space from " << WORD*(*itr)->first << " to " << WORD*(*itr)->last + 3 << endl;
			totalRamUsage -= ( (*itr)->last - (*itr)->first ) * WORD;
		}

		cout << dec << totalRamUsage << " out of " << DEFAULT_RAM << " bytes used." << endl;
		cout << "RAM is " << ( (double)totalRamUsage / totalRam ) * 100 << "% full" << endl; 
	}


private:
	b_size_t size;
	vector<byte_t> storage;
	list<Section*> blankSpaces;

	void InsertSpace(Section* space); // inserts a space in the blankSpaces list in a way that keeps the list ordered
};
