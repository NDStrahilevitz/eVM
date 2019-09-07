#ifndef VM_H_
#define VM_H_

#include "stdlib.h"
#include "stdint.h"

//opcodes definition
typedef enum{
    END = 0x0, NOP, 
    PUSHI, PUSHL, PUSHR, PUSHM, 
    POPR, POPM,
    MOVRI, MOVRL, MOVRR, MOVRM, MOVMI, MOVML, MOVMR, MOVMM,
    ADDRI, ADDRL, ADDRR, ADDRM, ADDMI, ADDML, ADDMR, ADDMM, 
    SUBRI, SUBRL, SUBRR, SUBRM, SUBMI, SUBML, SUBMR, SUBMM, 
    MULRI, MULRL, MULRR, MULRM, MULMI, MULML, MULMR, MULMM, 
    DIVRI, DIVRL, DIVRR, DIVRM, DIVMI, DIVML, DIVMR, DIVMM,
    IMULRI, IMULRL, IMULRR, IMULRM, IMULMI, IMULML, IMULMR, IMULMM, 
    IDIVRI, IDIVRL, IDIVRR, IDIVRM, IDIVMI, IDIVML, IDIVMR, IDIVMM,  
    CMPRI, CMPRL, CMPRR, CMPRM, CMPMI, CMPML, CMPMR, CMPMM,
    JMP, JNZ, JZ, JIG, JIL,
    NOTR, NOTM,
    ANDRI, ANDRL, ANDRR, ANDRM, ANDMI, ANDML, ANDMR, ANDMM, 
    ORRI, ORRL, ORRR, ORRM, ORMI, ORML, ORMR, ORMM,
    XORRI, XORRL, XORRR, XORRM, XORMI, XORML, XORMR, XORMM
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