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

	struct Section {		// A section of memory marked either as used or free
		int first;
		int last;
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

	int Allocate(byte_t data, int index);
	int Allocate(instruction_t data, int index);
	int AllocateChunk(queue<instruction_t>* instructions, int pid); // WARNING: This will "spin" until there is room in RAM to put it

	void Deallocate(int startIndex, size_t length);	// dealloc instructions starting at index
	// startIndex and length are both byte addresses (not instruction address)

	size_t Size() { return size; }
	instruction_t GetInstruction(int index);
	string GetStatus();

	Section* FirstAvailableSection(size_t instrNbr);

private:
	size_t size;
	vector<byte_t> storage;
	vector<Section*> blankSpaces;
};
