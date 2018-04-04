#include <string>

#include "RAM.hpp"

using namespace std;

/*** Allocation functions ***********************************************/
b_address_t RAM::Allocate(byte_t data, b_address_t index) {
	b_address_t address = index;

	if (size > index) {
		storage.at(index) = data;
	}
	else {
		throw "Invalid allocaion at: " + to_string(index) + ".";
		address = NULL_ADDRESS;
	}

	return address;
}

b_address_t RAM::Allocate(instruction_t data, b_address_t index) {
	byte_t temp = 0;
	b_address_t address = NULL_ADDRESS;

	temp = byte_t((data & 0xFF000000) >> (8 * 3));
	address = Allocate(temp, index);                // instr's address is location of first byte

	temp = byte_t((data & 0x00FF0000) >> (8 * 2));
	Allocate(temp, index + 1);

	temp = byte_t((data & 0x0000FF00) >> (8 * 1));
	Allocate(temp, index + 2);

	temp = byte_t((data & 0x000000FF) >> (8 * 0));
	Allocate(temp, index + 3);

	return address;
}

b_address_t RAM::AllocateChunk(queue<instruction_t>* instructions, int pid) {
	bool chunkInserted = false;
	bool sufficientSpace = true;
	b_address_t index;
	b_address_t startIndex;
	i_size_t size = instructions->size();
	Section* sect;

	while (!chunkInserted && sufficientSpace)
	{
		sect = FirstAvailableSection(size);
		
		// Indicate that RAM is full
		if (sect == NULL)
			return -1;

		/*** Critical section: Entry Point ***/
		sect->status = FLAGGED;					// Attempt to take control of C.S.
		sect->pid = pid;						// Keeps track of who actually took control

		if (sect->pid == pid)					// Verify the process calling this function has control of C.S.
		{
			/*** Critical section: Update section bounds ***/
			chunkInserted = true;
			index = sect->first * WORD;
			sect->first = sect->first + size;	// update lower Section bounds

			/*** Critical section: Exit point (releases unused resources) ***/
			if (sect->first == sect->last)		// remove a "space" whose size is 0
			{	bool removed = false;
				for (int i = 0; !removed && i < blankSpaces.size(); i++)
				{
					if (blankSpaces[i] == sect)
					{
						blankSpaces.erase(blankSpaces.begin() + i);
						removed = true;
					}
				}
			}
			else
				sect->status = FREE;

			// Safe to allocate instruction now
			startIndex = index;
			while (!instructions->empty()) {
				Allocate(instructions->front(), index);
				index += WORD;
				instructions->pop();
			}
		}
	}

	return startIndex;	// Returns the effective base (byte addressed)
}

/*** Deallocation functions **********************************************/


void RAM::Deallocate(b_address_t startIndex, b_size_t length)
{
	Section* prevSect;
	Section* currentSect;

	bool deallocated = false;

	// Case 1: Special case where space to deallocate is at beginning of memory
	if (startIndex == 0)
	{
		Section* sect = new Section;
		sect->first = 0;
		sect->last = length;
		sect->status = FREE;
	}
	else
	{
		// Case 2: Space to deallocate follows a blank space
		for (int i = 1; !deallocated && i < blankSpaces.size(); i++)
		{
			if ((blankSpaces.at(i)->last + 1) * WORD == startIndex)
			{
				Section* sect = blankSpaces.at(i);
				sect->last += length / WORD;
				sect->status = FREE;
			}
		}

		// Case 3: Space to deallocate must follow a used space
		if (!deallocated)
		{
			Section* sect = new Section;
			sect->first = startIndex / WORD;
			sect->last = sect->first + length / WORD;
			sect->status = FREE;
		}
	}
}

instruction_t RAM::GetInstruction(b_address_t index) {
	instruction_t instruct = 0;
	instruct |= ((instruction_t)storage[index + 0]) << (8 * 3); // Shift 3 bytes
	instruct |= ((instruction_t)storage[index + 1]) << (8 * 2);
	instruct |= ((instruction_t)storage[index + 2]) << (8 * 1);
	instruct |= ((instruction_t)storage[index + 3]) << (8 * 0);

	return instruct;

}

/*** MMU functions ***********************************************************/
RAM::Section* RAM::FirstAvailableSection(i_size_t instrNbr)
{
	for (i_address_t i = 0; i < blankSpaces.size(); i++)
	{
		Section* s = blankSpaces.at(i);
		i_size_t spaceSize = s->last - s->first;
		// Make sure section is big enough to fit given number of instructions
		//		AND that another process has not flagged it to enter it
		if (instrNbr <= spaceSize && s->status == FREE)
			return s;
	}
	
	return NULL;		// No available space found
}




string RAM::GetStatus() {
	return "RAM size: " + to_string(size);
}



