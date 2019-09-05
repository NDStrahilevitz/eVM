#include "stdio.h"
#include "stdlib.h"
#include "stack.h"
#define STACK_SIZE 0xfff
#define PROGRAM_SIZE 512

uint32_t program[PROGRAM_SIZE] = 
{0x0a0005dc,
0x020f002d,
0x021d0000,
0x0224000f,
0x0};

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

//define and init the general registers
uint32_t regs[REGS_NUMBER];

//stack base pointer
int* bp;

//instruction pointer
int* ip;

//stack pointer
uint32_t* sp;

//define stack
Stack* st;

int run = 1;

void init_vm(){
   // program = (int*)malloc(sizeof(int*)*PROGRAM_SIZE);
    st = stack_new(STACK_SIZE);
    bp = sp = st->arr;
    ip = program;
}

void free_vm(){
    stack_free(st);
    //free(program);
}

void cpu_cycle(){
    uint32_t instr = *ip++;
    uint32_t opcode = (instr >> 24 &0xff);
    switch(opcode){
        case END:
            run = 0;
            break;
        case NOP:
            break;
        case PUSH:{
            uint16_t val = instr & 0xffff;
            stack_push(st, val);
            sp = &st->arr[st->top];
            break;
        }
        case POP:{
            int dest = instr & 0xf;
            regs[dest] = stack_pop(st);
            sp = &st->arr[st->top];
            break;
        }
        case MOV:{
            int flag = instr >> 20 &0x3;
            int dest = instr >> 16 & 0xf;
            if(flag == 0){                
                uint16_t val = instr & 0xffff;
                regs[dest] = val;
            }
            else if(flag == 1){
                uint16_t val = *sp;
                regs[dest] = val;
            }
            else if(flag == 2){
                int src = instr & 0xf;
                regs[dest] = regs[src];
            }
            else{
                exit(INVALID_INSTRUCTION);
            }
            break;
        } 
    }
}

int main(){
    init_vm();
    while(run){
        cpu_cycle();
    }
    printf("%d", regs[R13]);
    free_vm();
    return 0;
}