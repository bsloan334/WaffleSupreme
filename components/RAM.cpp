#include <string>
#include <cassert>

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

instruction_t RAM::GetInstruction(b_address_t index) {
	instruction_t instruct = 0;
	instruct |= ((instruction_t)storage[index + 0]) << (8 * 3); // Shift 3 bytes
	instruct |= ((instruction_t)storage[index + 1]) << (8 * 2);
	instruct |= ((instruction_t)storage[index + 2]) << (8 * 1);
	instruct |= ((instruction_t)storage[index + 3]) << (8 * 0);

	return instruct;

}

b_address_t RAM::AllocateChunk(queue<instruction_t>* instructions, int pid) {
	bool chunkInserted = false;
	b_address_t index;
	b_address_t startIndex;
	i_size_t size = instructions->size();
	Section* sect;

	while (!chunkInserted)
	{
		sect = FirstAvailableSection(size);

		// Indicate that RAM is full
		if (sect == NULL)
			return NULL_ADDRESS;

		/*** Critical section: Entry Point ***/
		if (sect->lock.TestAndSet() == FREE)
		{
			/*** Critical section: Update section bounds ***/
			chunkInserted = true;
			index = sect->first * WORD;
			sect->first += size;	            // update lower Section bounds

			/*** Critical section: Exit point (releases unused resources) ***/
			if (sect->first == sect->last)		// remove a "space" whose size is 0
			{
				bool removed = false;
				for (list<Section*>::iterator itr = blankSpaces.begin();
					!removed && itr != blankSpaces.end();
					itr++)
				{
					if (*itr == sect)
					{
						itr = blankSpaces.erase(itr);
						removed = true;
					}
				}
			}
			else    // Section bounds updated, so section is now free for use
				sect->lock.ReleaseLock();

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
		Section* sect = new Section();
		sect->first = 0;
		sect->last = length / WORD - 1;
		sect->lock.ReleaseLock();

		InsertSpace(sect);
	}
	else
	{
		// Case 2: Space to deallocate follows a blank space
		for (list<Section*>::iterator itr = blankSpaces.begin();
			!deallocated && itr != blankSpaces.end();
			itr++)
		{
			if (((*itr)->last + 1) * WORD == startIndex)
			{
				Section* sect = *itr;
				sect->last += length / WORD;
				sect->lock.ReleaseLock();

				deallocated = true;
			}
		}

		// Case 3: Space to deallocate must follow a used space
		if (!deallocated)
		{
			Section* sect = new Section;
			sect->first = startIndex / WORD;
			sect->last = sect->first + length / WORD - 1;
			sect->lock.ReleaseLock();

			InsertSpace(sect);
		}
	}
}

/*** MMU functions ***********************************************************/
RAM::Section* RAM::FirstAvailableSection(i_size_t instrNbr)
{
	for (list<Section*>::iterator itr = blankSpaces.begin();
		itr != blankSpaces.end();
		itr++)
	{
		Section* s = *itr;
		i_size_t spaceSize = s->last - s->first;
		// Make sure section is big enough to fit given number of instructions
		//		AND that another process has not flagged it to enter it
		if (instrNbr <= spaceSize && s->lock.TestLock() == FREE)
			return s;
	}

	return NULL;		// No available space found
}

void RAM::InsertSpace(Section* space)
{
	// No blank spaces. Insert space as only element
	if (blankSpaces.empty())
	{
		blankSpaces.push_front(space);

	}
	else
	{
		// Try to insert space at the beginning or middle of blankSpaces list
		bool inserted = false;
		for (list<Section*>::iterator itr = blankSpaces.begin();
			!inserted && itr != blankSpaces.end();
			itr++)
		{
			if ((*itr)->first > space->first)
			{
				blankSpaces.insert(itr, space);
				inserted = true;

				// Check bounds of inserted space and current space (*itr)
				assert(space->last < (*itr)->first); // If this throws assertion, something went wrong somewhere else

				// Concatenate spaces if appropriate
				if (space->last != 0 && space->last == (*itr)->first - 1)
				{
					space->last = (*itr)->last;
					itr = blankSpaces.erase(itr);
				}

				itr--;
				assert(*itr == space);

				if (itr != blankSpaces.begin())
				{
					itr--;

					// Check bounds of inserted space with previous space
					if (space->first <= (*itr)->last)
						printAvailableSpace();
					assert(space->first > (*itr)->last);

					// Concatenate spaces if appropriate
					if ((*itr)->last == space->first - 1)
					{
						(*itr)->last = space->last;
						itr++;	// (points to space now)
						itr = blankSpaces.erase(itr);
					}
				}

			}
		}

		// Beginning/Middle insertion failed, so insert at end of blankSpace list
		if (!inserted)
		{
			blankSpaces.push_back(space);
			list<Section*>::iterator itr = blankSpaces.begin();
			while (*itr != space) itr++;
			itr--;							// now points to blank space before "space"

			// Concatenate spaces if appropriate
			if ((*itr)->last == space->first - 1)
			{
				(*itr)->last = space->last;
				itr++; // (points to space now)
				itr = blankSpaces.erase(itr);
			}
		}

	} // end if-else block
}


string RAM::GetStatus() {
	return "RAM size: " + to_string(size);
}



