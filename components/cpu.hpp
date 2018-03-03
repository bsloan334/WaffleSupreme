/************************************************************
CPU Module

Module 1 : DMA Channel
	switch read/write
		case 0
		case 1
	signal( go to Computer Only module )
	

Module 2 : Compute Only
	Loop until requires resources or is preempted
		Fetch instruction (critcal section: program must be loaded into ram)
		Decode instruction
		Increment PC (critical section: exclusive PC access req.)
		Execute instruction (critical section: requires exclusive accessd to registers & I/O buffers)
			Modifies 
		Break if waiting or preempted
			waiting - resource requested is not already available (usually 11 opcodes)
			preempted - dispatcher has signaled CPU to stop using shared memory
			
			(if break, make sure PCB entry is complete)


**************************************************************/


#ifndef __CPU__
#define __CPU__

#pragma once
#include <cstdlib>
/** #include "PCB.h"			// imported from PCB data structures
 **/
 
/*** WARNING: Backdoor is OPEN! */


namespace projos
{

	class CPU
	{
		public:
		
			/*** Constructor ********************************************/
			CPU(int32_t cpuId);
			// Postconditions: CPU initialized
			
			/*** Public Member Functions ********************************/	
			
			/** void CPU::runProcess( Process* &p )
			 **/
			/*
			// This is the control flow function of the cpu
			// Preconditions:  process_state contains the process's current state
			//				   either initialized by the Loader module or saved by
			//				     the CPU module last time the process was run
			// Postconditions: process has been run and finished or preempted,
			//				   control returned to CPU,
			//				   process state has been saved
			
			
			/*****************************************************************/
			/*** Private keyword removed to allow testing access**************/
			/*** DO NOT FORGET TO CLOSE THIS BACKDOOR!!!!!!!!!! **************/
			/*****************************************************************/
		
		
		// private:
		
			/*** Private Data Fields ************************************/
			int32_t  cpuId;				// ID of CPU
			
			// Information decoded from instruction
			int32_t type;				// 2-bits at beginning of instr indicating type
			int32_t opcode;				// opcode
			int32_t reg1, reg2, reg3;	// registers
			int32_t address;			// 16- or 32-bit address
			
			// Pointer to process's entry in the PCB
			/**	Process* process;		// pntr to process entry
			 **/
			/**	RegStruct* registers;	// pntr to register array
			 **/
			/**	Buffer* inBuffer;		// pntr to input buffer
			 **/
			/** Buffer* outBuffer;		// pntr to output buffer
			 **/
			/** Buffer* tmpBuffer;		// pntr to temp buffer (chache_
			 **/
			
			
			/*** Private Member Functions *******************************/
			
			int32_t Fetch(int32_t address);
			// Preconditions:  address is an absolute address within program buffer bounds
			// Postconditions: A copy of value stored at address has been returned
			
			void Decode(int32_t instr);
			// Preconditions:  instr is an instruction retrieved with fetch
			// Postconditions: instr has been decoded into following fields
			//					opcode, reg_1, reg_2, reg_3, and address
			//					(NOTE: some of these will be 0 if not used
			//					in instruction)
			
			void DMA();
			// Preconditions:  Current instruction has been decoded and instr is I/O instr
			// Postconditions: Processes I/O operations. When finished, calls signal
			
			
			void Execute();
			// Preconditions:  Opcode has been decoded from instruction
			//				   Other relevant fields have been decoded from
			//					 instruction and stored in private data fields
			// Postconditions: current instruction has been executed,
			//				   process state has been updated, and
			//				   all instruction fields reset to default values
			
			int32_t EffectiveAddress(int32_t logicalAddress);
			// Preconditions:  effective address in program section
			// Postconditions: effective address is returned
	};

	/*** Static variables used for reading hex instructions ***************/

	static const int32_t MEM_NULL = -1;
	 
	 
	// Test values for various bits in instruction
	static const int32_t INSTR_TYPE_TEST = 0xC0000000;		// 1st 2 bits
	static const int32_t OPCODE_TEST 	 = 0x3F000000;		// next 6 bits
	static const int32_t REG_1_TEST		 = 0x00F00000;		// next 4 bits
	static const int32_t REG_2_TEST		 = 0x000F0000;		// next 4 bits
	static const int32_t REG_3_TEST		 = 0x0000F000;		// next 4 bits
	static const int32_t ADDRESS16_TEST	 = 0x0000FFFF;		// last 16 bits
	static const int32_t ADDRESS24_TEST	 = 0x00FFFFFF;		// last 24 bits
	
	// Bits to right-shift each instruction field to get its value
	static const unsigned int INSTR_TYPE_SHIFT = 30;		// instr type shifts 30 bits right
	static const unsigned int OPCODE_SHIFT	   = 24;		// opcode shifts 24 bits right
	static const unsigned int REG_1_SHIFT	   = 20;		// reg 1 shifts 20 bits right
	static const unsigned int REG_2_SHIFT      = 16;	    // reg 2 shifts 16 bits right
	static const unsigned int REG_3_SHIFT      = 12;        // reg 3 shifts 12 bits right
		// Note: no need to shift an address

	/*** Different Instruction Types *****************************************/
	static const int32_t R  = 0x00000000;  // Arithmetic instruction type
	static const int32_t I  = 0x40000000;  // Conditional Branch & Immediate instruction type
	static const int32_t J  = 0x80000000;  // Unconditional Jump instruction type
	static const int32_t IO = 0xC0000000;  // Input/Output instruction type

	/*** Instruction Set Opcodes *********************************************/
	// Format [opcode] + [instr. Type Code]
	static const int32_t RD   = 0x00000000;
	static const int32_t WR   = 0x01000000;
	static const int32_t ST   = 0x02000000;
	static const int32_t LW   = 0x03000000;
	static const int32_t MOV  = 0x04000000;
	static const int32_t ADD  = 0x05000000;
	static const int32_t SUB  = 0x06000000;
	static const int32_t MUL  = 0x07000000;
	static const int32_t DIV  = 0x08000000;
	static const int32_t AND  = 0x09000000;
	static const int32_t OR   = 0x0A000000;
	static const int32_t MOVI = 0x0B000000;
	static const int32_t ADDI = 0x0C000000;
	static const int32_t MULI = 0x0D000000;
	static const int32_t DIVI = 0x0E000000;
	static const int32_t LDI  = 0x0F000000;
	static const int32_t SLT  = 0x10000000;
	static const int32_t SLTI = 0x11000000;
	static const int32_t HLT  = 0x12000000;
	static const int32_t NOP  = 0x13000000; // NOP does not have instr. type
	static const int32_t JMP  = 0x14000000;
	static const int32_t BEQ  = 0x15000000;
	static const int32_t BNE  = 0x16000000;
	static const int32_t BEZ  = 0x17000000;
	static const int32_t BNZ  = 0x18000000;
	static const int32_t BGZ  = 0x19000000;
	static const int32_t BLZ  = 0x1A000000;
	
	// OPCODE strings - for printing and testing use only
	static const char* OPCODES_STR[0x1B] =
	{
		"RD" /*00*/,  "WR"  /*01*/,  "ST"  /*02*/,  "LW"  /*03*/,  "MOV" /*04*/
		"ADD"/*05*/,  "SUB" /*06*/,  "MUL" /*07*/,  "DIV" /*08*/,  "AND" /*09*/,
		"OR" /*0A*/,  "MOVI"/*0B*/,  "ADDI"/*0C*/,  "MULI"/*0D*/,  "DIVI"/*0E*/,
		"LDI"/*0F*/,  "SLT" /*10*/,  "SLTI"/*11*/,  "HLT" /*12*/,  "NOP" /*13*/,
		"JMP"/*14*/,  "BEQ" /*15*/,  "BNE" /*16*/,  "BEZ" /*17*/,  "BNZ" /*18*/,
		"BGZ"/*19*/,  "BLZ" /*1A*/
	};

} // end namespace projos

#endif
