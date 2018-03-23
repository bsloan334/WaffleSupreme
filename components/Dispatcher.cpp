#pragma once

#include "Dispatcher.hpp"

using namespace std;

void Dispatcher::LoadProcessToCPU(Process* p, CPU* targetCPU)
{
	targetCPU->RunProcess(p);
}
