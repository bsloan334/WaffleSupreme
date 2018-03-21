#pragma once

#include <vector>
#include <string>

#include "Types.hpp"

//This makes the disk byte-addressable
#define DISK_SIZE 2048 * 4

using namespace std;

class Disk {

private:

    size_t used; // Variable for holding how much space has been used
    vector<byte_t> disk; 
    size_t size;
    
    
public:
    Disk(int size = DISK_SIZE) : disk(size), used(0), size(size) {};
    bool is_Full() { return size == used; }  //Boolean to see if the disk is full
    int Allocate(byte_t data); // Function for allocate 
    int Allocate(instruction_t data);
    
    //The index needs to be the location of the byte
    byte_t ReadByte(int index);
    instruction_t ReadInstruction(int index);
    
    string GetStatus();
    size_t GetUsed() {return size;}
    

};
