#pragma once
#include <stdint.h>

#define WORD 4
#define NULL_ADDRESS -1
#define CPU_NBR 5

using namespace std;

typedef uint32_t instruction_t; // Instruction type
typedef uint8_t byte_t;			// Byte type

typedef int i_address_t;		// Instruction address type
typedef int b_address_t;		// Byte address type

typedef size_t i_size_t;		// Instruction size type
typedef size_t b_size_t;		// Byte size type

typedef size_t f_index_t;		// Frame index type
typedef size_t f_size_t;		// Frame size type

typedef size_t p_index_t;		// Page index type
typedef size_t p_size_t;		// Page size type