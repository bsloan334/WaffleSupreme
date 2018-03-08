#pragma once
#include <cstdlib>
#include <iostream>
#include <list>
#include <cstdint>
using namespace std; 

class Data
{
public: 
	Data()
	{

	}
	Data(int inputSize, int outputSize, int tempSize) : 
		inputBufSize(inputSize),
		outputBufSize(outputSize),
		tempBufSize(tempSize)
	{
		inputBuffer = new int[inputSize];
		outputBuffer = new int[outputSize];
		tempBuffer = new int[tempSize];
	}
	
	int GetInputBufferSize() { return inputBufSize; }
	int GetOutputBufferSize() { return outputBufSize; }
	int GetTempBufferSize() { return tempBufSize; }
	void FillOutputBuffer(int input[]) { *outputBuffer = *input; }
	void FillTempBuffer(int input[]) { *tempBuffer = *input; }

private: 
	int *inputBuffer;
	int inputBufSize = 0;
	int *outputBuffer;
	int outputBufSize = 0;
	int *tempBuffer;
	int tempBufSize = 0;
};