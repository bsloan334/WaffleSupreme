#pragma once
#include <vector>
#include <deque>
#include <string>
#include "types.hpp"

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
    void Allocate(byte_t data); // Function for allocate 
    void Allocate(instruction_t data);
    
    //The index needs to be the location of the byte
    byte_t ReadByte(size_t index);
    instruction_t ReadInstruction(size_t index);
    
    string GetStatus();
    size_t GetUsed() {return size;}
    

};
