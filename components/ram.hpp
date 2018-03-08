#pragma once
#include <deque>
#include <vector>
#include "types.hpp"

//This makes the ram byte-addressable
#define DEAFULT_RAM 1024 * 4

using namespace std;

class RAM {

   private: 
      size_t size;
      vector<byte_t> storage;

   public:
      RAM(size_t size = DEAFULT_RAM) : size(size), storage(size, 0) {}

      void Allocate(byte_t data, int index);
      void Allocate(instruction_t data, int index);
      size_t Size() { return size; }

      instruction_t GetInstruction(size_t index);

      string GetStatus();

};
