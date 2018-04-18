#pragma once

#include <cstdlib>
#include <cstdint>

using namespace std; 

class Data
{
public: 
	Data(int inputSize = 0, int outputSize = 0, int tempSize = 0) //: 
		//inputBufSize(inputSize),
		//outputBufSize(outputSize),
		//tempBufSize(tempSize)
	{
        // Initialize Cache
		inputBufSize = inputSize;
        outputBufSize = outputSize;
        tempBufSize = tempSize;
	}

	Data(const Data &d)
	{
		inputBufSize = d.inputBufSize;
		outputBufSize = d.outputBufSize;
		tempBufSize = d.tempBufSize;
	}
	
	int GetInputBufferSize() { return inputBufSize; }
	int GetOutputBufferSize() { return outputBufSize; }
	int GetTempBufferSize() { return tempBufSize; }
    
    /** See note above. It is simplest just to let instruction set change the cache itself ***/
	//void FillOutputBuffer(int input[]) { *outputBuffer = *input; }
	//void FillTempBuffer(int input[]) { *tempBuffer = *input; }

private: 

    /** CPU uses sizes to verify data read and written from/to cache is done so in proper bounds **/
	//int inputBufSize = 0;
	//int outputBufSize = 0;
	//int tempBufSize = 0;
    int inputBufSize;
    int outputBufSize;
    int tempBufSize;
};