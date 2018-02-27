#pragma once
#include <CircularBuffer.hpp>
#include <Cache.hpp>
#include <Permissions.hpp>
#include <Register.hpp> 


#include <cstdlib>
#include <cstdint>
#include <iostream>

using namespace std;

// 
// Record of process environment that is saved on CPU interrupt
//
class State
{
public:
	State(int32_t pc, list<Register> registers, Permissions perm, Cache c, int32_t page) :
		programCounter(pc),
		stateRegisters(registers),
		permissions(perm),
		cache(c),
		activePages(page)
	{
	
	}
private:
	int32_t programCounter = 0;
	list<Register> stateRegisters;
	Permissions permissions;
	Cache cache;
	int32_t activePages = 0;
};
