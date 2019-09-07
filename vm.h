#ifndef VM_H_
#define VM_H_

#include "stdlib.h"
#include "stdint.h"

//opcodes definition
typedef enum{
    END = 0x0, NOP, 
    PUSHI, PUSHL, PUSHR, PUSHM, 
    POPM, POPR,
    MOVRI, MOVRL, MOVRM, MOVRR, MOVML, MOVMM, MOVMR,
    ADDRI, ADDRL, ADDRM, ADDRR,  ADDML, ADDMM, ADDMR, 
    SUBRI, SUBRL, SUBRM, SUBRR, SUBML, SUBMM, SUBMR, 
    MULRI, MULRL, MULRM, MULRR, MULML, MULMM, MULMR, 
    DIVRI, DIVRL, DIVRM, DIVRR, DIVML, DIVMM, DIVMR, 
    IMULRI, IMULRL, IMULRM, IMULRR, IMULML, IMULMM, IMULMR, 
    IDIVRI, IDIVRL, IDIVRM, IDIVRR, IDIVML, IDIVMM, IDIVMR,  
    CMPRI, CMPRL, CMPRM, CMPRR, CMPML, CMPMM, CMPMR, 
    JMP, JNZ, JZ, JG, JGE, JL, JLE, JA, JAE, JB, JBE,
    NOTM, NOTR,
    ANDRI, ANDRL, ANDRM, ANDRR, ANDML, ANDMM, ANDMR, 
    ORRI, ORRL, ORRM, ORRR, ORML, ORMM, ORMR,
    XORRI, XORRL, XORRM, XORRR, XORML, XORMM, XORMR
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

vm_t* vm_init();
void vm_destroy(vm_t* vm);

void vm_clear_flag(vm_t* vm, FLAG flag);
int vm_read_flag(const vm_t* vm, FLAG flag);
void vm_set_flag(vm_t* vm, FLAG flag);

void vm_run(vm_t* vm);
void vm_cpu_cycle(vm_t* vm);

#endif