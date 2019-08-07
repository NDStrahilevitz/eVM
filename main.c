#include "stdio.h"
#include "stdlib.h"
#include "stack.h"
#define STACK_SIZE 512
#define PROGRAM_SIZE 512

uint32_t program[PROGRAM_SIZE] = 
{0x200005dc,
0x4f010000,
0x4402000f,
0x0};

//define opcodes
typedef enum{
    END, //no arg
    NOP, //no arg
    PUSH, //one arg, value to push
    POP, //one arg, register to load to
    MOV, //two args, dest reg and src value or reg
    ADD, //two args
    SUB, //two args
    MUL, //two args
    DIV, //two args
    JMP, //one arg
    JNZ, //one arg
    TEST, //one arg
    NOT, //one arg
    AND, //two args
    OR, //two args
    XOR //two args
} OPCODE;

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
uint16_t regs[REGS_NUMBER];

//stack base pointer
int* bp;

//instruction pointer
int* ip;

//stack pointer
uint16_t* sp;

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
    free(st);
    //free(program);
}

//TODO: create scopes and rewrite the opcodes to be readable
void cpu_cycle(){
    uint32_t instr = *ip++;
    uint32_t opcode = (instr >> 28 &0xf);
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
            break;
        }
        case MOV:{
            int flag = instr >> 16 &0x3;
            int dest = instr >> 24 & 0xf;
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
    printf("%d", regs[R5]);
    return 0;
}