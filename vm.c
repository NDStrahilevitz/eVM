#include <stdlib.h>
#include <string.h>

#include "operations.h"
#include "vm.h"
#include "exitcodes.h"


vm_t* vm_create(){
    vm_t* new_vm = (vm_t*)malloc(sizeof(vm_t));

    memset(new_vm, 0, sizeof(vm_t));

    return new_vm;
}
void vm_destroy(vm_t* vm){
    free(vm);
}

void vm_clear_flag(vm_t* vm, FLAG flag){
    vm->registers[FL] &= ~(1U << flag); 
}

int vm_read_flag(const vm_t* vm, FLAG flag){
    return vm->registers[FL] >> flag & 1U;
}

void vm_set_flag(vm_t* vm, FLAG flag){
    vm->registers[FL] |= 1U << flag; 
}

void print_vm_details(vm_t* vm){
    printf("Registers Status:\n");
    printf("R0:     0x%08x\n", vm->registers[R0]);
    printf("R1:     0x%08x\n", vm->registers[R1]);
    printf("R2:     0x%08x\n", vm->registers[R2]);
    printf("R3:     0x%08x\n", vm->registers[R3]);
    printf("R4:     0x%08x\n", vm->registers[R4]);
    printf("R5:     0x%08x\n", vm->registers[R5]);
    printf("R6:     0x%08x\n", vm->registers[R6]);
    printf("R7:     0x%08x\n", vm->registers[R7]);
    printf("R8:     0x%08x\n", vm->registers[R8]);
    printf("R9:     0x%08x\n", vm->registers[R9]);
    printf("R10:    0x%08x\n", vm->registers[R10]);
    printf("R11:    0x%08x\n", vm->registers[R11]);
    printf("FL:     0x%08x\n", vm->registers[FL]);
    printf("PC:     0x%08x\n", vm->registers[PC]);
    printf("BP:     0x%08x\n", vm->registers[BP]);
    printf("SP:     0x%08x\n", vm->registers[SP]);

    printf("Flags Status:\n");
    printf("OF:     %d\n", vm_read_flag(vm, OF));
    printf("ZF:     %d\n", vm_read_flag(vm, ZF));
    printf("NF:     %d\n", vm_read_flag(vm, NF));
    printf("CF:     %d\n", vm_read_flag(vm, CF));
}

void read_program_to_mem(vm_t* vm, FILE* f){
    uint32_t* p = &vm->memory[0x8000];
    while (fscanf(f,"%x",p++)==1);      
}
void read_program_from_file(vm_t* vm, const char* path){
    FILE* f = fopen( path, "r");

    if(f == NULL){
        exit(FAILED_TO_READ_FILE);
    }
    
    read_program_to_mem(vm, f);
}

void vm_start(vm_t* vm){
    vm->running = 1;
    vm->registers[PC] = 0x8000;
}

void vm_run(vm_t* vm){
    while(vm->running)
        vm_cpu_cycle(vm);
}
void vm_cpu_cycle(vm_t* vm){
    uint32_t instruction = vm->memory[vm->registers[PC]++];
    uint32_t opcode = instruction >> 24 & 0xff;
    printf("PC: %x\n", vm->registers[PC]);
    printf("Instruction: %x Opcode: %u\n", instruction, opcode);
    switch(opcode){
        case END:{
            vm_end(vm);
            break;
        }
        
        case NOP:{
            vm_nop(vm);
            break;
        }

        case PUSHI:{
            uint32_t val = instruction & 0xfffff;
            vm_push(vm, val);
            break;
        }

        case PUSHL:{
            uint32_t val = vm->memory[vm->registers[PC]++];
            vm_push(vm, val);
            break;
        }

        case PUSHR:{
            uint32_t reg = (instruction >> 20) & 0xf;
            uint32_t val = vm->registers[reg];
            vm_push(vm, val);
            break;
        }

        case POP:{
            REGISTER reg = (instruction >> 20) & 0xf;
            vm_pop(vm, reg);
            break;
        }

        case LDR:{
            REGISTER dest = (instruction >> 20) & 0xf;
            REGISTER ptr = instruction & 0xf;
            vm_ldr(vm, dest, ptr);
            break;
        }

        case STR:{
            REGISTER ptr_reg = (instruction >> 20) & 0xf;
            REGISTER val_reg = instruction & 0xf;
            vm_str(vm, ptr_reg, val_reg);
            break;
        }

        case MOVRI:{
            vm_ri_op(vm, instruction, vm_mov);
            break;
        }

        case MOVRL:{
            vm_rl_op(vm, instruction, vm_mov);
            break;
        }

        case MOVRR:{
            vm_rr_op(vm, instruction, vm_mov);
            break;
        }

        case ADDRI:{
            vm_ri_op(vm, instruction, vm_add);
            break;
        }

        case ADDRL:{
            vm_rl_op(vm, instruction, vm_add);
            break;
        }

        case ADDRR:{
            vm_rr_op(vm, instruction, vm_add);
            break;
        }

        case SUBRI:{
            vm_ri_op(vm, instruction, vm_sub);
            break;
        }

        case SUBRL:{
            vm_rl_op(vm, instruction, vm_sub);
            break;
        }

        case SUBRR:{
            vm_rr_op(vm, instruction, vm_sub);
            break;
        }

        case MULRI:{
            vm_ri_op(vm, instruction, vm_mul);
            break;
        }

        case MULRL:{
            vm_rl_op(vm, instruction, vm_mul);
            break;
        }

        case MULRR:{
            vm_rr_op(vm, instruction, vm_mul);
            break;
        }

        case DIVRI:{
            vm_ri_op(vm, instruction, vm_div);
            break;
        }

        case DIVRL:{
            vm_rl_op(vm, instruction, vm_div);
            break;
        }

        case DIVRR:{
            vm_rr_op(vm, instruction, vm_div);
            break;
        }

        case IMULRI:{
            vm_ri_op(vm, instruction, vm_imul);
            break;
        }

        case IMULRL:{
            vm_rl_op(vm, instruction, vm_imul);
            break;
        }

        case IMULRR:{
            vm_rr_op(vm, instruction, vm_imul);
            break;
        }

        case IDIVRI:{
            vm_ri_op(vm, instruction, vm_idiv);
            break;
        }

        case IDIVRL:{
            vm_rl_op(vm, instruction, vm_idiv);
            break;
        }

        case IDIVRR:{
            vm_rr_op(vm, instruction, vm_idiv);
            break;
        }

        case CMPRI:{
            vm_ri_op(vm, instruction, vm_cmp);
            break;
        }

        case CMPRL:{
            vm_rl_op(vm, instruction, vm_cmp);
            break;
        }

        case CMPRR:{
            vm_rr_op(vm, instruction, vm_cmp);
            break;
        }

        case JMP:{
            uint32_t address = instruction & 0xffff;
            vm_jmp(vm, address, 1);
            break;
        }

        case JNZ:{
            uint32_t address = instruction & 0xffff;
            uint8_t cond = vm_read_flag(vm, ZF) == 0;
            vm_jmp(vm, address, cond);
            break;
        }

        case JZ:{
            uint32_t address = instruction & 0xffff;
            uint8_t cond = vm_read_flag(vm, ZF) == 1;
            vm_jmp(vm, address, cond);
            break;
        }

        case JG:{
            uint32_t address = instruction & 0xffff;
            uint8_t cond = vm_read_flag(vm, ZF) == 0 && (vm_read_flag(vm, NF) == vm_read_flag(vm, OF)) ;
            vm_jmp(vm, address, cond);
            break;
        }

        case JGE:{
            uint32_t address = instruction & 0xffff;
            uint8_t cond = vm_read_flag(vm, NF) == vm_read_flag(vm, OF) ;
            vm_jmp(vm, address, cond);
            break;
        }

        case JL:{
            uint32_t address = instruction & 0xffff;
            uint8_t cond = vm_read_flag(vm, NF) != vm_read_flag(vm, OF) ;
            vm_jmp(vm, address, cond);
            break;
        }

        case JLE:{
            uint32_t address = instruction & 0xffff;
            uint8_t cond = vm_read_flag(vm, ZF) == 0 || (vm_read_flag(vm, NF) != vm_read_flag(vm, OF)) ;
            vm_jmp(vm, address, cond);
            break;
        }

        case JA:{
            uint32_t address = instruction & 0xffff;
            uint8_t cond = vm_read_flag(vm, CF) == 0;
            vm_jmp(vm, address, cond);
            break;
        }

        case JAE:{
            uint32_t address = instruction & 0xffff;
            uint8_t cond = vm_read_flag(vm, CF) == 0 || vm_read_flag(vm, CF) == 1;
            vm_jmp(vm, address, cond);
            break;
        }

        case JB:{
            uint32_t address = instruction & 0xffff;
            uint8_t cond = vm_read_flag(vm, CF) == 1;
            vm_jmp(vm, address, cond);
            break;
        }

        case JBE:{
            uint32_t address = instruction & 0xffff;
            uint8_t cond = vm_read_flag(vm, CF) == 1 || vm_read_flag(vm, CF) == 1;
            vm_jmp(vm, address, cond);
            break;
        }

        case NOT:{
            const REGISTER reg = (instruction >> 20) & 0xf;
            vm_not(vm, reg);
            break;
        }

        case ANDRI:{
            vm_ri_op(vm, instruction, vm_and);
            break;
        }

        case ANDRL:{
            vm_rl_op(vm, instruction, vm_and);
            break;
        }

        case ANDRR:{
            vm_rr_op(vm, instruction, vm_and);
            break;
        }

        case ORRI:{
            vm_ri_op(vm, instruction, vm_or);
            break;
        }

        case ORRL:{
            vm_rl_op(vm, instruction, vm_or);
            break;
        }

        case ORRR:{
            vm_rr_op(vm, instruction, vm_or);
            break;
        }

        case XORRI:{
            vm_ri_op(vm, instruction, vm_xor);
            break;
        }

        case XORRL:{
            vm_rl_op(vm, instruction, vm_xor);
            break;
        }

        case XORRR:{
            vm_rr_op(vm, instruction, vm_xor);
            break;
        }

        default:
            exit(OPCODE_NOT_EXIST);
            break;

    }
}
