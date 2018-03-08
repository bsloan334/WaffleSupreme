#include <Process.hpp>

deque<Register*>* Process::GetRegisters()
{
	return *registers;
}