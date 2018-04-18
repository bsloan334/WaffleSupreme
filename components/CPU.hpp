#pragma once

#include <cstdlib>
#include <sstream>

#include "Process.hpp"
#include "RAM.hpp"
#include "Types.hpp"

using namespace std;

class CPU
{
    public:
    
        /*** Constructor ********************************************/
        CPU(RAM* ram, int cpuID);
        // Postconditions: CPU initialized
        
        /*** Public Member Functions ********************************/    
        
        bool RunProcess( Process* p ); // Dispatcher runs a process using this function
        // This is the control flow function of the cpu
        // Preconditions:  process_state contains the process's current state
        //                   either initialized by the Loader module or saved by
        //                   the CPU module last time the process was run
        // Postconditions: process has been run and finished or preempted,
        //                   control returned to CPU,
        //                   process state has been saved
        
    
        Process* GetCurrentProcess() const;
        // Postcondition:    ptr to currently running process is returned
    
    private:
    
        /*** Private Data Fields ************************************/
        int  cpuId;                    // ID of CPU
        
        // Information decoded from instruction
        instruction_t type;                // 2-bits at beginning of instr indicating type
        instruction_t opcode;            // opcode
        instruction_t reg1, reg2, reg3;    // registers
        instruction_t address;            // 16- or 32-bit address
        
        // Pointer to process's entry in the PCB
        Process* process;            // pntr to process entry
        RAM* ram;                    // pntr to OS's RAM
        b_address_t* pc;             // program counter
		int state;                   // process's current state (NEW, READY, RUNNING, WAITING, TERMINATED)
        instruction_t* registers;    // pntr to register array  /***** TO DO - MAKE 16 a Process Constant *****/
        b_address_t programBase;     // abs beginning address of program file
		i_size_t programSize;        // nbr of instructions in JOB section
		Cache* cache;                // pntr to process's cache data

		bool processComplete;		 // Whether or not the terminated process ran to completion
        bool processContinue;		 // Whether or not to continue running process
        
        // Stream for storing status statements
        stringstream output;
        
        
        /*** Private Member Functions *******************************/
        
        void Decode(instruction_t instr);
        // Preconditions:  instr is an instruction retrieved with fetch
        // Postconditions: instr has been decoded into following fields
        //                    opcode, reg_1, reg_2, reg_3, and address
        //                    (NOTE: some of these will be 0 if not used
        //                    in instruction)
        
        void DMA();
        // Preconditions:  Current instruction has been decoded and instr is I/O instr
        // Postconditions: Processes I/O operations. When finished, calls signal
        
        
        void Execute();
        // Preconditions:  Opcode has been decoded from instruction
        //                   Other relevant fields have been decoded from
        //                     instruction and stored in private data fields
        // Postconditions: current instruction has been executed,
        //                   process state has been updated, and
        //                   all instruction fields reset to default values
        
        b_address_t EffectiveAddress(b_address_t logicalAddress);
        // Preconditions:  effective address in program section
        // Postconditions: effective address is returned

		instruction_t Fetch(b_address_t address);
		// Preconditions:  address is an absolute address within program buffer bounds
		// Postconditions: A copy of value stored at address has been returned

		void Write(instruction_t data, b_address_t address);
		// Precondition:   address is within instruction set or cache
		// Postcondition:  instruction has been written to address


		void printRegs()
		{
			cout << "REGS:" << endl;
			for (int i = 0; i < NBR_OF_REGS; i++)
				cout << hex << "reg " << i << " = " << *(registers + i) << endl;
		}

};

/*** Static variables used for reading hex instructions ***************/

static const int EMPTY = -1;
 
 
// Test values for various bits in instruction
static const instruction_t INSTR_TYPE_TEST  = 0xC0000000;        // 1st 2 bits
static const instruction_t OPCODE_TEST            = 0x3F000000;        // next 6 bits
static const instruction_t REG_1_TEST            = 0x00F00000;        // next 4 bits
static const instruction_t REG_2_TEST            = 0x000F0000;        // next 4 bits
static const instruction_t REG_3_TEST            = 0x0000F000;        // next 4 bits
static const instruction_t ADDRESS16_TEST      = 0x0000FFFF;        // last 16 bits
static const instruction_t ADDRESS24_TEST      = 0x00FFFFFF;        // last 24 bits
 
// Bits to right-shift each instruction field to get its value
static const unsigned int INSTR_TYPE_SHIFT = 30;        // instr type shifts 30 bits right
static const unsigned int OPCODE_SHIFT       = 24;        // opcode shifts 24 bits right
static const unsigned int REG_1_SHIFT         = 20;        // reg 1 shifts 20 bits right
static const unsigned int REG_2_SHIFT      = 16;      // reg 2 shifts 16 bits right
static const unsigned int REG_3_SHIFT      = 12;    // reg 3 shifts 12 bits right
    // Note: no need to shift an address

/*** Different Instruction Types *****************************************/
static const instruction_t R  = 0x0;  // Arithmetic instruction type
static const instruction_t I  = 0x1;  // Conditional Branch & Immediate instruction type
static const instruction_t J  = 0x2;  // Unconditional Jump instruction type
static const instruction_t IO = 0x3;  // Input/Output instruction type

/*** Instruction Set Opcodes *********************************************/
static const instruction_t RD   = 0x00;
static const instruction_t WR   = 0x01;
static const instruction_t ST   = 0x02;
static const instruction_t LW   = 0x03;
static const instruction_t MOV  = 0x04;
static const instruction_t ADD  = 0x05;
static const instruction_t SUB  = 0x06;
static const instruction_t MUL  = 0x07;
static const instruction_t DIV  = 0x08;
static const instruction_t AND  = 0x09;
static const instruction_t OR   = 0x0A;
static const instruction_t MOVI = 0x0B;
static const instruction_t ADDI = 0x0C;
static const instruction_t MULI = 0x0D;
static const instruction_t DIVI = 0x0E;
static const instruction_t LDI  = 0x0F;
static const instruction_t SLT  = 0x10;
static const instruction_t SLTI = 0x11;
static const instruction_t HLT  = 0x12;
static const instruction_t NOP  = 0x13; // NOP does not have instr. type
static const instruction_t JMP  = 0x14;
static const instruction_t BEQ  = 0x15;
static const instruction_t BNE  = 0x16;
static const instruction_t BEZ  = 0x17;
static const instruction_t BNZ  = 0x18;
static const instruction_t BGZ  = 0x19;
static const instruction_t BLZ  = 0x1A;

// OPCODE strings - for printing and testing use only
static const char* OPCODES_STR[0x1B] =
{
    "RD" /*00*/,  "WR"  /*01*/,  "ST"  /*02*/,  "LW"  /*03*/,  "MOV" /*04*/,
    "ADD"/*05*/,  "SUB" /*06*/,  "MUL" /*07*/,  "DIV" /*08*/,  "AND" /*09*/,
    "OR" /*0A*/,  "MOVI"/*0B*/,  "ADDI"/*0C*/,  "MULI"/*0D*/,  "DIVI"/*0E*/,
    "LDI"/*0F*/,  "SLT" /*10*/,  "SLTI"/*11*/,  "HLT" /*12*/,  "NOP" /*13*/,
    "JMP"/*14*/,  "BEQ" /*15*/,  "BNE" /*16*/,  "BEZ" /*17*/,  "BNZ" /*18*/,
    "BGZ"/*19*/,  "BLZ" /*1A*/
};


/*** Error Messages ***/
static const char* ERR_R_INSTR_MISMATCH = "Error: Instruction ID-ed as Arithmetic (R) did not match any known R instruction.";
static const char* ERR_I_INSTR_MISMATCH = "Error: Instruction ID-ed as Immediate (I) did not match any known I instruction.";
static const char* ERR_J_INSTR_MISMATCH = "Error: Instruction ID-ed as Unconditional Jump (J) did not match any known J instruction.";
static const char* ERR_IO_INSTR_MISMATCH = "Error: Instruction ID-ed as Input/Ouput (I/O) did not match any known I/O instruction.";
static const char* ERR_INSTR_MISMATCH = "Error: Instruction did not match known instruction.";
static const char* ERR_DIV_BY_0 = "Error: active process attempted division by 0, but the CPU knows better!";

static const char* ERR_OUT_OF_BOUNDS = "Error: Process tried to access address outside given process bounds.";