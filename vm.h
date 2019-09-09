#ifndef VM_H_
#define VM_H_

#include "stdint.h"

//opcodes definition
typedef enum{
    END = 0x0, NOP, 
    PUSHI, PUSHL, PUSHR,
    POP,
    LDR, STR,
    MOVRI, MOVRL, MOVRR,
    ADDRI, ADDRL, ADDRR,
    SUBRI, SUBRL, SUBRR,
    MULRI, MULRL, MULRR,
    DIVRI, DIVRL, DIVRR,
    IMULRI, IMULRL, IMULRR,
    IDIVRI, IDIVRL, IDIVRR, 
    CMPRI, CMPRL, CMPRR,
    JMP, JNZ, JZ, JG, JGE, JL, JLE, JA, JAE, JB, JBE,
    NOT,
    ANDRI, ANDRL, ANDRR,
    ORRI, ORRL, ORRR,
    XORRI, XORRL, XORRR
} OPCODE;

/*
    R0 - ACCUMULATOR
    R1 - ACCUMULTOR
    FL - FLAGS
    PC - PROGRAM COUNTER 
    BP - BASE POINTER
    SP - STACK POINTER
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
    FL,
    PC,
    BP,
    SP,
    REGS_NUMBER
} REGISTER;

typedef enum{
    OF,
    ZF,
    NF,
    CF
} FLAG;

typedef struct vm{
    uint32_t memory[0xffff];
    uint32_t registers[REGS_NUMBER];
    int running;
} vm_t;

vm_t* vm_create();
void vm_destroy(vm_t* vm);

void vm_clear_flag(vm_t* vm, FLAG flag);
int vm_read_flag(const vm_t* vm, FLAG flag);
void vm_set_flag(vm_t* vm, FLAG flag);

void print_vm_details(vm_t* vm);

void vm_run(vm_t* vm);
void vm_cpu_cycle(vm_t* vm);

#endif