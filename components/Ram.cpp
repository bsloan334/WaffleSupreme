#include "stdafx.h"
#include <string>

#include "RAM.hpp"

using namespace std;

int RAM::Allocate(byte_t data, int index) {
   int address = index;
   
   if (size > index) {
      storage.at(index) = data;
   } else {
      throw "Invalid allocaion at: " + to_string(index) + ".";
      address = -1;   // essentially NULL address is returned
   }
   
   return address;
}

int RAM::Allocate(instruction_t data, int index) {
   byte_t temp = 0;
   int address = -1;

	temp = byte_t((data & 0xFF000000) >> (8*3));
	address = Allocate(temp, index);                // instr's address is location of first byte

	temp = byte_t((data & 0x00FF0000) >> (8*2));
	Allocate(temp, index + 1);

	temp = byte_t((data & 0x0000FF00) >> (8*1));
	Allocate(temp, index + 2);

	temp = byte_t((data & 0x000000FF) >> (8*0));
	Allocate(temp, index + 3);
    
    return address;
}

instruction_t RAM::GetInstruction(int index) {
	instruction_t instruct = 0;
	instruct |= ((instruction_t)storage[index+0]) << (8 * 3); // Shift 3 bytes
	instruct |= ((instruction_t)storage[index+1]) << (8 * 2);
	instruct |= ((instruction_t)storage[index+2]) << (8 * 1);
	instruct |= ((instruction_t)storage[index+3]) << (8 * 0);

	return instruct;

}

string RAM::GetStatus() {
   return "RAM size: " + to_string(size);
}



