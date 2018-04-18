#include <cstdint>
#include <cassert>

#include "CPU.hpp"            // class header file

using namespace std;

/*** Constructor ********************************************/
CPU::CPU(RAM* ram_init, int cpuId_init)
// Postconditions: cpu initialized with unique CPU ID
{
	cpuId = cpuId_init;
	ram = ram_init;
	process = NULL;
	processContinue = false;
}

/*** Public Member Functions ********************************/

bool CPU::RunProcess(Process* p)

// This is the control flow function of the cpu
// Preconditions:  process_state contains the process's current state
//                   either initialized by the Loader module or saved by
//                     the CPU module last time the process was run
// Postconditions: process has been run and finished or preempted,
//                   control returned to CPU,
//                   process state has been saved
{
	output.str("");                            // clear output stream

	process = p;
	registers = process->Registers();
	pc = process->ProgramCounter();				// Logical byte address
	programBase = process->GetProgramBase();	// Absolute byte address
	programSize = process->GetProgramSize();	// Size (number of instructions) of program
	cache = process->GetCache();

	/*** Critical Section: Entry Point ***/
	while (process->CheckState() != RUNNING);  // Make sure that process is in RAM and marked RUNNING by long term

	processContinue = true;


	cout << "Program base is " << programBase << endl;
	cout << "Program size/Cache offset is " << programSize << endl;
	assert(cache != NULL);


	/*** Critical Section: Run process ***/
	while (processContinue)
	{
		b_address_t instrAddress = programBase + *pc;	// Get address of current instr
		instruction_t instr = Fetch(instrAddress);		// Fetch instruction from RAM

		Decode(instr);								// Decode instruction, updating instruction information fields

		printRegs();		// For testing only

		// IO operation: run DMA channel
		if (type == IO)
			DMA();

		// Execute instruction, using instruction information fields
		else
			Execute();
	}

	/*** Critical Section: Exit Point ***/
	process->SetState(TERMINATED);

	// Print process's output cache here
	cout << "Process " << p->GetID() << " output dump: " << endl;

	cache->PrintOutput();

	ram->Deallocate(programBase, programSize*WORD);

	return true;
}

Process* CPU::GetCurrentProcess() const
// Postcondition:    ptr to currently running process is returned
{
	return process;    // NULL if no process currently running
}

/*** Private Member Functions *******************************/

void CPU::DMA()
// Preconditions : Current instruction has been decoded and instr is I/O instr
// Postconditions: Processes I/O operations. When finished, calls signal
{

	instruction_t src1 = EMPTY;
	instruction_t src2 = EMPTY;
	instruction_t src3 = EMPTY;
	instruction_t* dst1 = NULL;
	instruction_t* dst2 = NULL;
	instruction_t* dst3 = NULL;

	// Store values in each register that is not empty
	if (reg1 != EMPTY)
	{
		src1 = *(registers + reg1);
		dst1 = registers + reg1;
	}

	if (reg2 != EMPTY)
	{
		src2 = *(registers + reg2);
		dst2 = registers + reg2;
	}
	if (reg3 != EMPTY)
	{
		src3 = *(registers + reg3);
		dst3 = registers + reg3;
	}

	switch (opcode)
	{
		case RD:    // read: src2 = value @ address or src2 = src1
			if (address != 0)
				*dst1 = Fetch(EffectiveAddress(address));
			else
			{
				*dst1 = Fetch(EffectiveAddress(src2));
			}
		
			*pc += WORD;
			break;

		case WR:    // write: value @ address = src2 or reg1 = src2
			//   constraint: address limited to temp buffer or output buffer

			if (address != 0) // case 1: address field contains output dest
				Write(src1, EffectiveAddress(address));

			else // case 2: reg2 contains output dest
				Write(src1, EffectiveAddress(src2));
			*pc += WORD;
			break;

		default:
			output << ERR_IO_INSTR_MISMATCH << endl;
			processContinue = false;
			processComplete = false;
	}
}

void CPU::Decode(instruction_t instr)
// Preconditions:  instr is an instruction retrieved with fetch
// Postconditions: instr has been decoded into following fields
//                    opcode, reg_1, reg_2, reg_3, and address
//                    (NOTE: some of these will be 0 if not used
//                    in instruction)
{
	type = instr & INSTR_TYPE_TEST;    // Get instr type from instr
	opcode = instr & OPCODE_TEST;    // Get opcode from instr

	// Shift each field right by appropriate nbr of bits to gets its actual value
	type = type >> INSTR_TYPE_SHIFT;
	opcode = opcode >> OPCODE_SHIFT;

	switch (type)
	{
	case R:     // Arithmetic Instr ---- args: 3 registers, no address
		reg1 = instr & REG_1_TEST;
		reg2 = instr & REG_2_TEST;
		reg3 = instr & REG_3_TEST;
		address = EMPTY;
		break;
	case I:  // Immediate Instr ----- args: 2 registers, 1 16-bit address
		reg1 = instr & REG_1_TEST;
		reg2 = instr & REG_2_TEST;
		reg3 = EMPTY;
		address = instr & ADDRESS16_TEST;
		break;
	case J:     // Branch instr -------- args: 1 24-bit address
		reg1 = EMPTY;
		reg2 = EMPTY;
		reg3 = EMPTY;
		address = instr & ADDRESS24_TEST;
		break;
	case IO: // I/O instr ----------- args: 2 registers, 1 16-bit address
		reg1 = instr & REG_1_TEST;
		reg2 = instr & REG_2_TEST;
		reg3 = EMPTY;
		address = instr & ADDRESS16_TEST;
		break;
	default:
		reg1 = EMPTY;
		reg2 = EMPTY;
		reg3 = EMPTY;
		address = EMPTY;

	} // end switch

	// (Only shift registers if they are not Null)
	if (reg1 != EMPTY)
		reg1 = reg1 >> REG_1_SHIFT;

	if (reg2 != EMPTY)
		reg2 = reg2 >> REG_2_SHIFT;

	if (reg3 != EMPTY)
		reg3 = reg3 >> REG_3_SHIFT;

} // end Decode

void CPU::Execute()
// Preconditions:  Opcode has been decoded from instruction
//                   Other relevant fields have been decoded from
//                     instruction and stored in private data fields
// Postconditions: current instruction has been executed,
//                   process state has been updated, and
//                   all instruction fields reset to default values
{
	instruction_t src1 = EMPTY;
	instruction_t src2 = EMPTY;
	instruction_t src3 = EMPTY;
	instruction_t* dst1 = NULL;
	instruction_t* dst2 = NULL;
	instruction_t* dst3 = NULL;

	// Store values in each register that is not empty
	if (reg1 != EMPTY)
	{
		src1 = *(registers + reg1);
		dst1 = registers + reg1;
	}

	if (reg2 != EMPTY)
	{
		src2 = *(registers + reg2);
		dst2 = registers + reg2;
	}
	if (reg3 != EMPTY)
	{
		src3 = *(registers + reg3);
		dst3 = registers + reg3;
	}

	if (type == R)            // Arithmetic instruction format
	{
		cout << "opcode=" << hex << opcode << endl;

		switch (opcode)
		{
		case MOV:        // move: reg1 = src2
			*dst1 = src2;
			*pc += WORD;
			break;

		case ADD:        // add: dest = (src1+src2)
			*dst3 = src1 + src2;
			*pc += WORD;
			break;

		case SUB:        // subtract: dest = (src1-src2)
			*dst3 = src1 - src2;
			*pc += WORD;
			break;

		case MUL:        // multiply: dest = (src1*src2)
			*dst3 = src1 * src2;
			*pc += WORD;
			break;

		case DIV:        // divide: dest = (src1/src2)
			if (src2 == 0) // (constraint: src2 != 0)
			{
				output << ERR_DIV_BY_0 << endl;
				processContinue = false;
				processComplete = false;
				break;
			}

			*dst3 = (instruction_t)(src1 / src2);  // WARNING: This will be truncated to uint32_t
			*pc += WORD;
			break;

		case AND:        // bitwise and: dest = (src1&src2)
			*dst3 = src1 & src2;
			*pc += WORD;
			break;

		case OR:        // bitwise or: dest = (src1|src2)
			*dst3 = src1 | src2;
			*pc += WORD;
			break;

		case SLT:       // set left test: dest =
						//         1 if src1 < src2
						//         0 if src1 >= src2
			if (src1 < src2)
				*dst3 = 1;
			else
				*dst3 = 0;
			*pc += WORD;
			break;

		default:
			output << ERR_R_INSTR_MISMATCH << endl;
		}

	}
	else if (type == I)    // Conditional Branch and Immediate format
	{
		switch (opcode)
		{
		case ST:        // set data: copy src1 value to address @ src2
			Write(src1, EffectiveAddress(src2));
			*pc += WORD;
			break;

		case LW:        // load data: copy contents of address @ src1 to src2
			*dst2 = Fetch(EffectiveAddress(src1));
			*pc += WORD;
			break;

		case MOVI:        // move: copy data/address in src1 to src2
			*dst2 = address;
			*pc += WORD;
			break;

		case ADDI:        // add: src2 += address
			*dst2 += address;
			*pc += WORD;
			break;

		case MULI:        // mul: src2 *= address
			*dst2 *= address;
			*pc += WORD;
			break;

		case DIVI:        // div: src2 /= address
			//         constraint : address != 0
			if (address == 0)
			{
				output << ERR_DIV_BY_0 << endl;
				processContinue = false;
				processComplete = false;
				break;
			}

			*dst2 /= address;  // WARNING: value will be truncated
			*pc += WORD;
			break;

		case LDI:        // load: src2 = address
			*dst2 = address;
			*pc += WORD;
			break;

		case SLTI:        // set on left test: src2 = 
			//        1 if src1 < address
			//        0 if src1 >= address
			if (src1 < address)
				*dst2 = 1;
			else
				*dst2 = 0;
			*pc += WORD;
			break;

		case BEQ:        // branch if equal: set pc to logical address if src1 == src2
			if (src1 == src2)
				*pc = address;
			else
				*pc += WORD;
			break;

		case BNE:        // branch if not equal: set pc to logical address if src1 != src2
			if (src1 != src2)
				*pc = address;
			else
				*pc += WORD;
			break;

		case BEZ:        // branch if zero: set pc to logical address if src1 == 0
			if (src1 == 0)
				*pc = address;
			else
				*pc += WORD;
			break;

		case BNZ:        // branch if not zero: set pc to logical address if src1 != 0
			if (src1 != 0)
				*pc = address;
			else
				*pc += WORD;
			break;

		case BGZ:        // branch if greater than zero: set pc to logical address if src1 > 0
			if (src1 > 0)
				*pc = address;
			else
				*pc += WORD;
			break;

		case BLZ:        // branch less than zero: set pc to logical address if src1 < 0
			if (src1 < 0)
				*pc = address;
			else
				*pc += WORD;
			break;

		default:        // error
			output << ERR_I_INSTR_MISMATCH << endl;
		}

	}
	else if (type == J)    // Unconditional Jump format
	{
		switch (opcode)
		{
		case HLT:        // signal execution completion
			processContinue = false;
			processComplete = true;
			break;

		case JMP:        // set pc unconditionally to given logical address
			*pc = address;
			break;

		default:        // error
			output << ERR_J_INSTR_MISMATCH << endl;
		}

	}
	else if (opcode == NOP)
	{
		*pc += WORD;

	}
	else
	{
		output << ERR_INSTR_MISMATCH << endl;
		processContinue = false;
		processComplete = false;
	}
}

instruction_t CPU::Fetch(b_address_t address)
// Preconditions:  Address is an absolute address within instruction set or cache
// Postconditions: A copy of value stored at address has been returned either from RAM or from Cache
{
	b_address_t programEnd = programBase + programSize*WORD;
	bool inInstructionSet = (address >= programBase && address < programEnd);
	bool inCache = (address >= programEnd && address < programEnd + cache->Size()*WORD);

	instruction_t instr = -1;

	if (inInstructionSet)
	{
		instr = ram->GetInstruction(address);
	}

	else if (inCache)
	{
		i_address_t cacheAddress = (address - programBase - programSize*WORD) / WORD;
		instr = cache->Read(cacheAddress);
	}

	else
	{
		output << ERR_OUT_OF_BOUNDS << endl;
		processContinue = false;
		processComplete = false;
	}

	return instr;
}

void CPU::Write(instruction_t data, b_address_t address)
// Precondition:   address is within instruction set or cache
// Postcondition:  instruction has been written to address
{
	/*** Calculate all acceptable program and cache bounds ***/
	b_address_t programEnd = programBase + programSize*WORD;
	b_address_t outputStart = programEnd + cache->GetOutputOffset()*WORD;
	b_address_t outputEnd = outputStart + cache->GetOutputBuffSize()*WORD;
	b_address_t tempStart = programEnd + cache->GetTempOffset()*WORD;
	b_address_t tempEnd = tempStart + cache->GetTempBuffSize()*WORD;

	bool inInstructionSet = (address >= programBase && address < programEnd);
	bool inOutput = (address >= outputStart && address < outputEnd);
	bool inTemp = (address >= tempStart && address < tempEnd);

	assert(inInstructionSet || inOutput || inTemp);		// Ensure that write is within bounds

	if (inInstructionSet)
		ram->Allocate(data, address);
	else
		cache->Write(data, ((address - programBase) / WORD) - programSize);

}

b_address_t CPU::EffectiveAddress(b_address_t logicalAddress)
// Preconditions:  effective address in program section
// Postconditions: effective address is returned
{
	b_address_t effAddress = programBase + logicalAddress;

	// Check precondition
	assert(effAddress < programBase || effAddress >= programBase + programSize);

	return effAddress;
}