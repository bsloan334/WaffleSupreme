#pragma once

#include <iostream>
#include <vector>
#include <cstdlib>
#include <cstdint>

using namespace std;

class Register
{
public:
	Register(string n, int32_t val, int32_t s, int32_t resetVal) :
		name(n),
		value(val), 
		size(s),
		resetValue(resetVal)
	{
	
	}
private:
	string name;
	int32_t value = 0;
	int32_t size = 1;
	int32_t resetValue = 0;
};

