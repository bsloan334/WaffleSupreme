#pragma once
#include <Process.hpp>

#include <cstdlib>
#include <iostream>
#include <list>
#include <circularBuffer.hpp>
#include <register.hpp>
#include <cstdint>

class PCB
{ 
public:
	PCB(string n) : 
		name(n)
	{
		
	}
private:
	string name;
	list<Process> processes;

};
