#pragma once

#include <vector>
#include <string>
#include <queue>

#include "Types.h"

//This makes the disk byte-addressable
#define DISK_SIZE 2048 * WORD

using namespace std;

class Disk {

private:

	b_size_t used; // Variable for holding how much space has been used
	vector<byte_t> disk;
	b_size_t size;


public:
	Disk(int size = DISK_SIZE) : disk(size), used(0), size(size) {};
	bool is_Full() { return size == used; }  //Boolean to see if the disk is full
	b_address_t Allocate(byte_t data); // Function for allocate 
	b_address_t Allocate(instruction_t data);

	//The index needs to be the location of the byte
	byte_t ReadByte(b_address_t index);
	instruction_t ReadInstruction(b_address_t index);
	queue<instruction_t> ReadInstructionChunk(b_address_t index, i_size_t size);

	string GetStatus();
	b_size_t GetUsed() { return size; }


};
