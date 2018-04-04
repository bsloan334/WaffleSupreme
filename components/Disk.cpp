#pragma once

#include <vector>
#include <deque>

#include "Disk.hpp"
#include "Types.hpp"

using namespace std;

b_address_t Disk::Allocate(byte_t data){
	b_address_t address = NULL_ADDRESS;

	if (is_Full()){
		throw "The disk is full, try again";
	}
	else {
		disk[used] = data;
		address = used;
		used++;
	}

	return address;
}

b_address_t Disk::Allocate(instruction_t data) {
	byte_t temp = 0;
	b_address_t address = NULL_ADDRESS;

	temp = byte_t((data & 0xFF000000) >> (8 * 3));
	address = Allocate(temp);

	temp = byte_t((data & 0x00FF0000) >> (8 * 2));
	Allocate(temp);

	temp = byte_t((data & 0x0000FF00) >> (8 * 1));
	Allocate(temp);

	temp = byte_t((data & 0x000000FF) >> (8 * 0));
	Allocate(temp);

	return address;
}

instruction_t Disk::ReadInstruction(b_address_t index) {
	instruction_t instruct = 0;
	instruct |= (instruction_t)disk[index + 0] << (8 * 3); // 3 bytes
	instruct |= (instruction_t)disk[index + 1] << (8 * 2);
	instruct |= (instruction_t)disk[index + 2] << (8 * 1);
	instruct |= (instruction_t)disk[index + 3] << (8 * 0);

	return instruct;
}

queue<instruction_t> Disk::ReadInstructionChunk(b_address_t index, i_size_t size) {
	queue<instruction_t> target;
	instruction_t temp;
	b_address_t targetClone = index;

	for (; index < targetClone + size*WORD; index += WORD) {
		temp = 0;
		temp |= ((instruction_t)disk[index + 0]) << (8 * 3);
		temp |= ((instruction_t)disk[index + 1]) << (8 * 2);
		temp |= ((instruction_t)disk[index + 2]) << (8 * 1);
		temp |= ((instruction_t)disk[index + 3]) << (8 * 0);
		target.push(temp);
	}
	return target;
}

string Disk::GetStatus() {
	return "Disk size: " + to_string(size) + "\n Disk used: " + to_string(used);
}
