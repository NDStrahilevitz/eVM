#include "stdio.h"
#include "stdlib.h"
#include "stack.h"

#ifndef VM_H_
#define VM_H_

//define opcodes
typedef enum{
    END = 0, NOP, MOV, 
    PUSH = 10, POP, 
    ADD = 20, SUB, MUL, DIV, 
    TEST = 30, JMP, JNZ, JZ,
    NOT = 40, AND, OR, XOR,
    PRINT = 50
} OPCODE;

/*
    R0 - ACCUMULATOR
    R1 - ACCUMULTOR
    R12 - FLAGS
 */
typedef enum{
    R0,
    R1,
    R2,
    R3,
    R4,
    R5,
    R6,
    R7,
    R8,
    R9,
    R10,
    R11,
    R12,
    R13,
    R14,
    R15,
    REGS_NUMBER
} REGISTER;

typedef struct vm{
    uint32_t memory[0xffff];
    Stack* stack;
    uint32_t registers[REGS_NUMBER];
    uint32_t* p_counter;
    uint32_t* st_ptr;
} VM;

#endif