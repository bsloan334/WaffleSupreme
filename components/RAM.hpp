#pragma once

#include <vector>
#include <iostream>

#include "Types.hpp"

/*** Changes made by Ruth...
All Allocate() functions now return an integer address
indicating the offset where the data was stored
[This is so the Loader and LongTerm can store the
appropriate base addresses in Processes they store]

******/

//This makes the ram byte-addressable
#define DEAFULT_RAM 1024 * WORD

using namespace std;

class RAM {

   private: 
      size_t size;
      vector<byte_t> storage;

   public:
      RAM(size_t size = DEAFULT_RAM) : size(size), storage(size, 0) {}

      int Allocate(byte_t data, int index);
      int Allocate(instruction_t data, int index);
      size_t Size() { return size; }

      instruction_t GetInstruction(int index);

      string GetStatus();
};
