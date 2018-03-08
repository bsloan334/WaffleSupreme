#pragma once

#include <iostream>
#include <vector>
#include <cstdlib>
#include <cstdint>

using namespace std;

class Register
{
public:
	Register()
	{

	}
	Register(string n, int resetVal) :
		name(n),
		resetValue(resetVal)
	{
	
	}
private:
	string name;
	instruction_t buffer[16];
	instruction_t resetValue = 0;
};

