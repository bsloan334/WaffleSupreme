#pragma once
#include <cstdlib>
#include <sstream>
#include "Process.hpp"
#include "RAM.hpp"
#include "Types.hpp"
 
/*** WARNING: Backdoor is OPEN! */

class CPU
{
	public:
	
		/*** Constructor ********************************************/
		CPU(int cpuId, RAM* ram);
		// Postconditions: CPU initialized
		
		/*** Public Member Functions ********************************/	
		
		void CPU::RunProcess( Process* p ); // Dispatcher runs a process using this function
		/*
		// This is the control flow function of the cpu
		// Preconditions:  process_state contains the process's current state
		//				   either initialized by the Loader module or saved by
		//				     the CPU module last time the process was run
		// Postconditions: process has been run and finished or preempted,
		//				   control returned to CPU,
		//				   process state has been saved
		
		*/
	
		Process* GetCurrentProcess() const;
		// Postcondition:	ptr to currently running process is returned
	
	private:
	
		/*** Private Data Fields ************************************/
		int  cpuId;				// ID of CPU
		RAM* ram;
		
		// Information decoded from instruction
		instruction_t type;				// 2-bits at beginning of instr indicating type
		instruction_t opcode;			// opcode
		instruction_t reg1, reg2, reg3;	// registers
		instruction_t address;			// 16- or 32-bit address
		
		// Pointer to process's entry in the PCB
		Process* process;			// pntr to process entry
		RAM* ram;					// pntr to OS's RAM
		int pc;						// program counter
		instruction_t* registers;	// pntr to register array  /***** TO DO - MAKE 16 a Process Constant *****/
		int outBuffer;				// offset of output buffer
		int tmpBuffer;				// offset of temp buffer (chache)
		int programBase;			// abs beginning address of program file
		
		
		// Whether or not to continue running process
		bool processContinue;
		
		// Stream for storing status statements
		stringstream output;
		
		
		/*** Private Member Functions *******************************/
		
		instruction_t Fetch(int address);
		// Preconditions:  address is an absolute address within program buffer bounds
		// Postconditions: A copy of value stored at address has been returned
		
		void Decode(instruction_t instr);
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
		
		int EffectiveAddress(int logicalAddress);
		// Preconditions:  effective address in program section
		// Postconditions: effective address is returned
		
		void RemoveProcess();
		// Postcondition:  the process state has been saved
		//				   the process cache has been saved
};

/*** Static variables used for reading hex instructions ***************/

static const int EMPTY = -1;
 
 
// Test values for various bits in instruction
static const instruction_t INSTR_TYPE_TEST  = 0xC0000000;		// 1st 2 bits
static const instruction_t OPCODE_TEST 	 	  = 0x3F000000;		// next 6 bits
static const instruction_t REG_1_TEST		    = 0x00F00000;		// next 4 bits
static const instruction_t REG_2_TEST		    = 0x000F0000;		// next 4 bits
static const instruction_t REG_3_TEST		    = 0x0000F000;		// next 4 bits
static const instruction_t ADDRESS16_TEST	  = 0x0000FFFF;		// last 16 bits
static const instruction_t ADDRESS24_TEST	  = 0x00FFFFFF;		// last 24 bits
 
// Bits to right-shift each instruction field to get its value
static const unsigned int INSTR_TYPE_SHIFT = 30;		// instr type shifts 30 bits right
static const unsigned int OPCODE_SHIFT	   = 24;		// opcode shifts 24 bits right
static const unsigned int REG_1_SHIFT	     = 20;		// reg 1 shifts 20 bits right
static const unsigned int REG_2_SHIFT      = 16;	  // reg 2 shifts 16 bits right
static const unsigned int REG_3_SHIFT      = 12;    // reg 3 shifts 12 bits right
	// Note: no need to shift an address

/*** Different Instruction Types *****************************************/
static const instruction_t R  = 0x00000000;  // Arithmetic instruction type
static const instruction_t I  = 0x40000000;  // Conditional Branch & Immediate instruction type
static const instruction_t J  = 0x80000000;  // Unconditional Jump instruction type
static const instruction_t IO = 0xC0000000;  // Input/Output instruction type

/*** Instruction Set Opcodes *********************************************/
static const instruction_t RD   = 0x00000000;
static const instruction_t WR   = 0x01000000;
static const instruction_t ST   = 0x02000000;
static const instruction_t LW   = 0x03000000;
static const instruction_t MOV  = 0x04000000;
static const instruction_t ADD  = 0x05000000;
static const instruction_t SUB  = 0x06000000;
static const instruction_t MUL  = 0x07000000;
static const instruction_t DIV  = 0x08000000;
static const instruction_t AND  = 0x09000000;
static const instruction_t OR   = 0x0A000000;
static const instruction_t MOVI = 0x0B000000;
static const instruction_t ADDI = 0x0C000000;
static const instruction_t MULI = 0x0D000000;
static const instruction_t DIVI = 0x0E000000;
static const instruction_t LDI  = 0x0F000000;
static const instruction_t SLT  = 0x10000000;
static const instruction_t SLTI = 0x11000000;
static const instruction_t HLT  = 0x12000000;
static const instruction_t NOP  = 0x13000000; // NOP does not have instr. type
static const instruction_t JMP  = 0x14000000;
static const instruction_t BEQ  = 0x15000000;
static const instruction_t BNE  = 0x16000000;
static const instruction_t BEZ  = 0x17000000;
static const instruction_t BNZ  = 0x18000000;
static const instruction_t BGZ  = 0x19000000;
static const instruction_t BLZ  = 0x1A000000;

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
