
#include <stdint.h>
#include "CPU.hpp"			// class header file
//#include "memory.h" 		// getMemVal(int address) function
					// 		and MEM_NULL constant
			

using namespace projos;			
					
/*** Constructor ********************************************/
CPU::CPU(int32_t cpuId_init)
// Postconditions: cpu initialized with unique CPU ID
{
	cpuId = cpuId_init;
	
	/**	process = NULL;
	 **/
	/**
		registers = NULL;
		inBuffer = NULL;
		outBuffer = NULL;
		tmpBuffer = NULL;
		
	 **/
}

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
{
	process = p;
	registers = p->GetRegisters();
	inBuffer = p->GetInputBuffer();
	outBuffer = p->GetOutputBuffer();
	tmpBuffer = p->GetTmpBuffer();
	
	while(true)
	{
		int instrAddress;
		int instr;
		
		// Get address of current instr
		instrAddress = process->programBase + process->pc;
	
		// Fetch instruction from RAM
		instr = Fetch(instrAddress);
		
		///Decode instruction, updating instruction information fields
		Decode(instr);
		
		// IO operation: run DMA channel
		if(type == IO)
			DMA();
		
		// Execute instruction, using instruction information fields
		else
			Execute();
	}
}
*/


/*** Private Member Functions *******************************/

void CPU::DMA()
// Preconditions:  Current instruction has been decoded and instr is I/O instr
// Postconditions: Processes I/O operations. When finished, calls signal
{
	if(type != IO)
	{
		// indicate error
		return;
	}
	
	switch(opcode)
	{
		case RD:	// read: reg1 = value @ address
			
			break;
		case WR:	// write: value @ address = reg1
					//   constraint: address limited to temp buffer or output buffer
			
			break;
		default:
			; // indicate error
			break;
	}
}

int32_t CPU::Fetch(int32_t address)
// Preconditions:  Address is an absolute address with program buffer bounds
//				   section_id is one of the program section types
// Postconditions: A copy of value stored at address has been returned
{
	// Make sure that data requested is within process section (buffer)
	
	
	// Get memory value
	/**GetMemVal(address);
	 **/
}

void CPU::Decode(int32_t instr)
// Preconditions:  instr is an instruction retrieved with fetch
// Postconditions: instr has been decoded into following fields
//					opcode, reg_1, reg_2, reg_3, and address
//					(NOTE: some of these will be 0 if not used
//					in instruction)
{
	type = instr & INSTR_TYPE_TEST;	// Get instr type from instr
	opcode = instr & OPCODE_TEST;	// Get opcode from instr
	
	switch(type)
	{
		case R:	 // Arithmetic Instr ---- args: 3 registers, no address
			reg1 = instr & REG_1_TEST;
			reg2 = instr & REG_2_TEST;
			reg3 = instr & REG_3_TEST;
			address = MEM_NULL;
			break;
		case I:  // Immediate Instr ----- args: 2 registers, 1 16-bit address
			reg1 = instr & REG_1_TEST;
			reg2 = instr & REG_2_TEST;
			reg3 = MEM_NULL;
			address = instr & ADDRESS16_TEST;
			break;
		case J:	 // Branch instr -------- args: 1 24-bit address
			reg1 = MEM_NULL;
			reg2 = MEM_NULL;
			reg3 = MEM_NULL;
			address = instr & ADDRESS24_TEST;
			break;
		case IO: // I/O instr ----------- args: 2 registers, 1 16-bit address
			reg1 = instr & REG_1_TEST;
			reg2 = instr & REG_2_TEST;
			reg3 = MEM_NULL;
			address = instr & ADDRESS16_TEST;
			break;
		default:
			reg1 = MEM_NULL;
			reg2 = MEM_NULL;
			reg3 = MEM_NULL;
			address = MEM_NULL;
			
	} // end switch
	
	// Shift each field right by appropriate nbr of bits to gets its actual value
	type = type >> INSTR_TYPE_SHIFT;	
	opcode = opcode >> OPCODE_SHIFT;
	
	// (Only shift registers if they are not Null)
	if(reg1 != MEM_NULL)
		reg1 = reg1 >> REG_1_SHIFT;		
	
	if(reg2 != MEM_NULL)
		reg2 = reg2 >> REG_2_SHIFT;		
	
	if(reg3 != MEM_NULL)
		reg3 = reg3 >> REG_3_SHIFT;		
	
} // end Decode

void CPU::Execute()
// Preconditions:  Opcode has been decoded from instruction
//				   Other relevant fields have been decoded from
//					 instruction and stored in private data fields
// Postconditions: current instruction has been executed,
//				   process state has been updated, and
//				   all instruction fields reset to default values
{
	int32_t arg1, arg2;
	
	if( type == R )			// Arithmetic instruction format
	{		
		switch(opcode)
		{
			case MOV:		// move: reg1 = reg2
				// process->SetReg(reg1, reg2);
				break;
		
			case ADD:		// add: reg3 = (reg1+reg2)
				// arg1 = process->GetReg(reg1, 0);
				// arg2 = process->GetReg(reg2, 0);
				// process->SetReg(reg3, arg1+arg2);
				break;
				
			case SUB:		// subtract: reg3 = (reg1-reg2)
				// arg1 = process->GetReg(reg1, 0);
				// arg2 = process->GetReg(reg2, 0);
				// process->SetReg(reg3, arg1+arg2);
				break;
				
			case MUL:		// multiply: reg3 = (reg1*reg2)
				// arg1 = process->GetReg(reg1, 0);
				// arg2 = process->GetReg(reg2, 0);
				// process->SetReg(reg3, arg1*arg2);
				break;
				
			case DIV:		// divide: reg3 = (reg1/reg2)
							// (constraint: reg2 != 0)
				// arg1 = process->GetReg(reg1, 0);
				// arg2 = process->GetReg(reg2, 0);
				// process->SetReg(reg3, arg1/arg2);
				
				// ADD constraint error handling
				
				break;
				
			case AND:		// bitwise and: reg3 = (reg1&reg2)
				// arg1 = process->GetReg(reg1, 0);
				// arg2 = process->GetReg(reg2, 0);
				// process->SetReg(reg3, arg1&arg2);
				break;
				
			case OR:		// bitwise or: reg3 = (reg1|reg2)
				// arg1 = process->GetReg(reg1, 0);
				// arg2 = process->GetReg(reg2, 0);
				// process->SetReg(reg3, arg1|arg2);
				break;
				
			case SLT:		// set left test: reg3 =
							// 		1 if reg1 < reg2
							// 		0 if reg1 >= reg2
				// arg1 = process->GetReg(reg1, 0);
				// arg2 = process->GetReg(reg2, 0);
				// if(arg1 < arg2)
				//	process->SetReg(reg3, 1);
				// else
				//	process->SetReg(reg3, 0);
				
				break;
				
			default:
				// throw error
				;
		}
		
	} else if( type == I )	// Conditional Branch and Immediate format
	{
		switch(opcode)
		{
			case ST:		// set data: copy reg1 value to address @ reg2
				
				break;
				
			case LW:		// load data: copy contents of address @ reg1 to reg2
				break;
				
			case MOVI:		// move: copy data/address in reg1 to reg2
				break;
				
			case ADDI:		// add: ??????????
							//	case (reg1 is empty): add address to address in reg2
							//	case (reg1 is 0??? ): reg1 = reg2 + address ????????????????
			break;
				
			case MULI:		// mul: ??????????
				break;
				
			case DIVI:		// div: ??????????
				break;
				
			case LDI:		// load: reg2 = value stored in address (load logical address into register)
				break;
				
			case SLTI:		// set on left test: reg2 = 
							//		1 if reg1 < address
							//		0 if reg1 >= address
				break;
				
			case BEQ:		// branch if equal: set pc to logical address if
							//		reg1 == reg2
				break;
				
			case BNE:		// branch if not equal: set pc to logical address if
							// 		reg1 != reg2
				break;
				
			case BEZ:		// branch if zero: set pc to logical address if
							//		reg1 == 0
				break;
				
			case BNZ:		// branch if not zero: set pc to logical address if
							// 		reg1 != 0
				break;
				
			case BGZ:		// branch if greater than zero: set pc to logical address if
							// 		reg1 > 0
				break;
				
			case BLZ:		// branch less than zero: set pc to logical address if
							//		reg1 < 0
				break;
			
			default:		// error
			;
		}
		
	} else if( type == J )	// Unconditional Jump format
	{	
		switch(opcode)
		{
			case HLT:		// signal execution completion
				// change process state in process's PCB entry
				break;
			
			case JMP:		// set pc unconditionally to given logical address
				// process->SetPC(address);
				break;
				
			default:		// error
			;
		}
		
	} else
	{
		// throw exception?
	}
}

int32_t EffectiveAddress(int32_t logicalAddress)
// Preconditions:  effective address in program section
// Postconditions: effective address is returned
{
	/**
	if(logicalAddress >= process->GetProgramSize())
	{	//indicate error
		return;
	}
	
	return (process->GetBaseAddress() + logicalAddress);
	
	*/
}
