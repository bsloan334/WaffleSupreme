#pragma once

#include "Types.hpp"

#include <cassert>

class Cache
{
	public:
		Cache(i_size_t inSize = 0, i_size_t outSize = 0, i_size_t tmpSize = 0);
		~Cache();

		/************* Functions for Reading from Buffers *************/
		instruction_t ReadInputBuff(i_address_t i);
		instruction_t ReadOutputBuff(i_address_t i);  // SAMI's function
		instruction_t ReadTempBuff(i_address_t i);

		instruction_t Read(i_address_t i);
		// Returns the ith instruction in the given buffer (INSTRUCITON ADDRESSABLE!!!)

		/************* Functions for Writing to Buffers *************/
		void Write(instruction_t instr, i_address_t i);

		/*************** Getter functions *************/
		i_size_t GetInputBuffSize();
		i_size_t GetOutputBuffSize();
		i_size_t GetTempBuffSize();
		i_size_t Size();

		i_size_t GetInputOffset();
		i_size_t GetOutputOffset();
		i_size_t GetTempOffset();

		/************** Utilities **********************/
		void PrintInput();
		void PrintOutput();
		void PrintTemp();

	private:
		instruction_t* buffer;
		i_size_t cacheSize;

		// Buffer sizes
		i_size_t inputBuffSize;
		i_size_t outputBuffSize;
		i_size_t tempBuffSize;

		// Buffer offsets from beginning of cache
		i_size_t inputOffset;
		i_size_t outputOffset;
		i_size_t tempOffset;
};