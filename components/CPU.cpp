#pragma once

#include <cstdint>

#include "CPU.hpp"            // class header file

using namespace std;
                    
/*** Constructor ********************************************/
CPU::CPU(int cpuId_init, RAM* ram_init)
// Postconditions: cpu initialized with unique CPU ID
{
    cpuId = cpuId_init;
    ram = ram_init;
    
    process = NULL;
    pc = EMPTY;
    registers = NULL;
    outBuffer = EMPTY;
    tmpBuffer = EMPTY;
     
    processContinue = false;
}

/*** Public Member Functions ********************************/

void CPU::RunProcess( Process* p)

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
    pc = process->GetProgramCounter();
    registers = process->Registers();
    outBuffer = process->GetOutputBase();
    tmpBuffer = process->GetTempBase();
    programBase = process->GetProgramBase();    // NEED
    state = process->GetState();
    
    while( process->GetState() != READY );  // Make sure that process is in READY queue 
    
    state->SetState(RUNNING);
    
    while(processContinue)
    {
        instruction_t instr;
        
        processContinue = true;
        
        // Get address of current instr
        instrAddress = process->programBase + process->pc;
    
        // Fetch instruction from RAM
        instr = Fetch(instrAddress);
    **/
        instruction_t instr = testProgram[*(pc)];
    
        // Decode instruction, updating instruction information fields
        Decode(instr);
        output << " Instruction decoded." << endl;
        
        // IO operation: run DMA channel
        if(type == IO)
        {    DMA();
            output << " Executed as I/O instr." << endl;
        }
        // Execute instruction, using instruction information fields
        else
        {    Execute();
            output << " Executed as Compute Only instr." << endl;
        }
        
        *(pc)++;
    }
    std::cout << output;
    process = NULL;
}

process& GetCurrentProcess() const
// Postcondition:    ptr to currently running process is returned
{
    return process;    // NULL if no process currently running
}

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
        case RD:    // read: src2 = value @ address or src2 = src1
            
            if(address != 0)
                *(registers + reg2) = Fetch(EffectiveAddress(address));
            else
                *(registers + reg2) = src1_val;
            
            *pc++;// increment PC
            
            break;
        case WR:    // write: value @ address = src2 or reg1 = src2
                    //   constraint: address limited to temp buffer or output buffer
            // TODO: Check that constraint is adhered to
            
            if(address != 0) // case 1: address field contains output dest
                ram->Allocate(src1_val, EffectiveAddress(address));
                
            else // case 2: reg2 contains output dest
                ram->Allocate(src1_val, EffectiveAddress(src2_val));
                
            break;
        default:
            ; // indicate error
            break;
    }
}

instruction_t CPU::Fetch(int address)
// Preconditions:  Address is an absolute address with program buffer bounds
//                   section_id is one of the program section types
// Postconditions: A copy of value stored at address has been returned
{
    // TODO: Make sure that data requested is within process section (buffer)
    
    return ram->GetInstruction(address);
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
    
    switch(type)
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
    
    // Shift each field right by appropriate nbr of bits to gets its actual value
    type = type >> INSTR_TYPE_SHIFT;    
    opcode = opcode >> OPCODE_SHIFT;
    
    // (Only shift registers if they are not Null)
    if(reg1 != EMPTY)
        reg1 = reg1 >> REG_1_SHIFT;        
    
    if(reg2 != EMPTY)
        reg2 = reg2 >> REG_2_SHIFT;        
    
    if(reg3 != EMPTY)
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
    instruction_t src1_val, src2_val;
    instruction_t* dest;
    
    // Store values in each register that is not empty
    if(reg1 != EMPTY)
        src1_val = *(registers + reg1);
    
    if(reg2 != EMPTY)
        src2_val = *(registers + reg2);
    
    if(reg3 != EMPTY)
        dest = (registers + reg3);
    
    if( type == R )            // Arithmetic instruction format
    {        
        switch(opcode)
        {
            case MOV:        // move: reg1 = src2
            
                *(registers + reg1)[0] = src2_val;
                break;
        
            case ADD:        // add: dest = (src1+src2)
            
                *dest = src1_val + src2_val;
                break;
                
            case SUB:        // subtract: dest = (src1-src2)
            
                *dest = src1_val - src2_val;
                break;
                
            case MUL:        // multiply: dest = (src1*src2)
            
                *dest = src1_val * src2_val;
                break;
                
            case DIV:        // divide: dest = (src1/src2)
                            // (constraint: src2 != 0)
                
                if(src2_val == 0)
                    ; // terminate process with error message (no division by 0)
                
                *dest = src1_val / src2_val;
                break;
                
            case AND:        // bitwise and: dest = (src1&src2)
                
                *dest = src1_val & src2_val;
                break;
                
            case OR:        // bitwise or: dest = (src1|src2)
                
                *dest = src1_val | src2_val;
                break;
                
            case SLT:        // set left test: dest =
                            //         1 if src1 < src2
                            //         0 if src1 >= src2
                
                if(src1_val < src2_val)
                    *dest = 1;
                else
                    *dest = 0;
                
                break;
                
            default:
                // throw error
                ;
        }
        
    } else if( type == I )    // Conditional Branch and Immediate format
    {
        switch(opcode)
        {
            case ST:        // set data: copy src1 value to address @ src2
                ram->Allocate(src1_val, EffectiveAddress(src2_val));
                break;
                
            case LW:        // load data: copy contents of address @ src1 to src2
                *(registers + reg2) = Fetch(EffectiveAddress(src1_val));
                break;
                
            case MOVI:        // move: copy data/address in src1 to src2
                *(registers + reg2) = src1_val;
                break;
                
            case ADDI:        // add: src2 += address
                *(registers + reg2) += address;
                break;
                
            case MULI:        // mul: src2 *= address
                *(registers + reg2) *= address;
                break;
                
            case DIVI:        // div: src2 /= address
                            //         constraint : address != 0
                if(address == 0)
                    ;    // terminate process with error and indicate division by 0
                break;
                
            case LDI:        // load: src2 = value stored in address (load logical address into register)
                *(registers + reg2) = Fetch(EffectiveAddress(address));
                break;
                
            case SLTI:        // set on left test: src2 = 
                            //        1 if src1 < address
                            //        0 if src1 >= address
                if(src1_val < address)
                    *(registers + reg2) = 1;
                else 
                    *(registers + reg2) = 0;
                break;
                
            case BEQ:        // branch if equal: set pc to logical address if
                            //        src1 == src2
                if(src1_val == src2_val)
                    *pc = address;
                break;
                
            case BNE:        // branch if not equal: set pc to logical address if
                            //         src1 != src2
                if(src_val1 != src_val2)
                    *pc = address;
                break;
                
            case BEZ:        // branch if zero: set pc to logical address if
                            //        src1 == 0
                if(src1_val == 0)
                    *pc = address;
                break;
                
            case BNZ:        // branch if not zero: set pc to logical address if
                            //         src1 != 0
                if(src1_val != 0)
                    *pc = address;
                break;
                
            case BGZ:        // branch if greater than zero: set pc to logical address if
                            //         src1 > 0
                if(src1_val > 0)
                    *pc = address;
                break;
                
            case BLZ:        // branch less than zero: set pc to logical address if
                            //        src1 < 0
                if(src1_val < 0)
                    *pc = address;
                break;
            
            default:        // error
            ;
        }
        
    } else if( type == J )    // Unconditional Jump format
    {    
        switch(opcode)
        {
            case HLT:        // signal execution completion
                processContinue = false;
                removeProcess();
                break;
            
            case JMP:        // set pc unconditionally to given logical address
                *pc = address;
                break;
                
            default:        // error
            ;
        }
        
    } else
    {
        // throw exception?
    }
}

int EffectiveAddress(int logicalAddress)
// Preconditions:  effective address in program section
// Postconditions: effective address is returned
{
    return programBase + logicalAddress;
}
