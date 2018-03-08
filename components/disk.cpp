#include <vector>
#include "disk.hpp"
#include "types.hpp"

using namespace std;

void Disk::Allocate(byte_t data){
    if (is_Full){
       throw "The disk is full, try again";
    } else {
       disk[used] = data;
       used++;
    }
}

void Disk::Allocate(instruction_t data) {
   byte_t temp = 0;

	temp = byte_t((data & 0xFF000000) >> (8*3));
	Allocate(temp);

	temp = byte_t((data & 0x00FF0000) >> (8*2));
	Allocate(temp);

	temp = byte_t((data & 0x0000FF00) >> (8*1));
	Allocate(temp);

	temp = byte_t((data & 0x000000FF) >> (8*0));
	Allocate(temp);
}

instruction_t Disk::ReadInstruction(int index) {
   instruction_t instruct = 0;
	instruct |= (instruction_t)disk[index + 0] << (8*3); // 3 bytes
	instruct |= (instruction_t)disk[index + 1] << (8*2);
	instruct |= (instruction_t)disk[index + 2] << (8*1);
	instruct |= (instruction_t)disk[index + 3] << (8*0);

	return instruct;
}

string Disk::GetStatus() {
   return "Disk size: " + to_string(size) + "\n Disk used: " + to_string(used);
}
