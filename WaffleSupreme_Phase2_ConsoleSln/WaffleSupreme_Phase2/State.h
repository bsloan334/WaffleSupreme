#pragma once
#include "CircularBuffer.h"
#include "Cache.h"
#include "Permissions.h"
#include "Register.h"


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
	State()
	{

	}
	State(int newState, int pc, list<Register> registers, Permissions perm, Cache c, int page) :
		currentState(newState),
		programCounter(pc),
		stateRegisters(registers),
		permissions(perm),
		cache(c),
		activePages(page)
	{

	}
private:
	int currentState;
	int programCounter = 0;
	list<Register> stateRegisters;
	Permissions permissions;
	Cache cache;
	int activePages = 0;
};
