#include "Cache.hpp"


Cache::Cache(i_size_t inSize, i_size_t outSize, i_size_t tmpSize)
{
	inputBuffSize = inSize;
	outputBuffSize = outSize;
	tempBuffSize = tmpSize;

	inputOffset = 0;
	outputOffset = inputBuffSize;
	tempOffset = inputBuffSize + outputBuffSize;
	cacheSize = inputBuffSize + outputBuffSize + tempBuffSize;

	buffer = new instruction_t[cacheSize];
}

Cache::~Cache() { delete[] buffer; }

/************* Functions for Reading from Buffers *************/
instruction_t Cache::ReadInputBuff(i_address_t i)
{
	assert(i >= 0 && i < inputBuffSize);
	return Read(inputOffset + i);
}

// SAMI's function
instruction_t Cache::ReadOutputBuff(i_address_t i)
{
	assert(i >= 0 && i < outputBuffSize);
	return Read(outputOffset + i);
}

instruction_t Cache::ReadTempBuffer(i_address_t i)
{
	assert(i >= 0 && i < tempBuffSize);
	return Read(tempOffset + i);
}

instruction_t Cache::Read(i_address_t i) { return *(buffer + i); }
// Returns the ith instruction in the given buffer (INSTRUCITON ADDRESSABLE!!!)

/************* Functions for Writing to Buffers *************/
void Cache::Write(instruction_t instr, i_address_t i)
{
	assert(i >= 0 && i < cacheSize);
	*(buffer + i) = instr;
}

/*************** Getter functions *************/
i_size_t Cache::GetInputBuffSize() { return inputBuffSize; }
i_size_t Cache::GetOutputBuffSize() { return outputBuffSize; }
i_size_t Cache::GetTempBuffSize() { return tempBuffSize; }
i_size_t Cache::Size() { return inputBuffSize + outputBuffSize + tempBuffSize; }

i_size_t Cache::GetInputOffset() { return inputOffset; }
i_size_t Cache::GetOutputOffset() { return outputOffset; }
i_size_t Cache::GetTempOffset() { return tempOffset; }