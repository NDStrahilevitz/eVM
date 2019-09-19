#include <stdlib.h>

#include "operations.h"
#include "exitcodes.h"

#define STACK_END 0x1fff

void vm_end(vm_t* vm){
    vm->running = 0;
}

void vm_nop(vm_t* vm){
    ++vm->memory[PC];
}

void vm_push(vm_t* vm, const uint32_t val){
    uint32_t sp_loc = vm->registers[SP]++;
    if(sp_loc > STACK_END){
        exit(SEGMENTATION_FAULT);
    }
    vm->memory[sp_loc] = val;
}

void vm_pop(vm_t* vm, const REGISTER dest){
    uint32_t sp_loc = vm->registers[SP]--;
    if(sp_loc == 0)
        exit(SEGMENTATION_FAULT);
    uint32_t val = vm->memory[sp_loc - 1];
    vm->memory[sp_loc - 1] = 0;
    vm->registers[dest] = val;
}

void vm_mov(vm_t* vm, const REGISTER dest, const uint32_t arg){
    vm->registers[dest] = arg;
}

void vm_ldr(vm_t* vm, const REGISTER dest, const REGISTER ptr_reg){
    vm->registers[dest] = vm->memory[vm->registers[ptr_reg]];
}

void vm_str(vm_t* vm, const REGISTER ptr_reg, const REGISTER val_reg){
    vm->memory[vm->registers[ptr_reg]] = vm->registers[val_reg];
}

void vm_add(vm_t* vm, const REGISTER dest, const uint32_t arg){
    uint32_t prevVal = vm->registers[dest];
    uint32_t res = vm->registers[dest] += arg;
    if(res >> 31)
        vm_set_flag(vm, NF);
    else
        vm_clear_flag(vm, NF);
    if(res < prevVal)
        vm_set_flag(vm, CF);
    else
        vm_clear_flag(vm, CF);
    if(prevVal >> 31  == arg >> 31 && res >> 31 != prevVal >> 31)
        vm_set_flag(vm, OF);
    else
        vm_clear_flag(vm, OF);
    if(res == 0)
        vm_set_flag(vm, ZF);
    else
        vm_clear_flag(vm, ZF);
}

void vm_sub(vm_t* vm, const REGISTER dest, const uint32_t arg){
    uint32_t prevVal = vm->registers[dest];
    uint32_t res = vm->registers[dest] -= arg;
    if(res >> 31)
        vm_set_flag(vm, NF);
    else
        vm_clear_flag(vm, NF);
    if(arg > prevVal)
        vm_set_flag(vm, CF);
    else
        vm_clear_flag(vm, CF);
    if(prevVal >> 31  != arg >> 31 && arg >> 31 == res >> 31)
        vm_set_flag(vm, OF);
    else
        vm_clear_flag(vm, OF);
    if(res == 0)
        vm_set_flag(vm, ZF);
    else
        vm_clear_flag(vm, ZF);
}

void vm_mul(vm_t* vm, const REGISTER dest, const uint32_t arg){
    exit(NOT_YET_IMPLEMENTED);
}

void vm_imul(vm_t* vm, const REGISTER dest, const uint32_t arg){
    exit(NOT_YET_IMPLEMENTED);
}

void vm_div(vm_t* vm, const REGISTER dest, const uint32_t arg){
    exit(NOT_YET_IMPLEMENTED);
}

void vm_idiv(vm_t* vm, const REGISTER dest, const uint32_t arg){
    exit(NOT_YET_IMPLEMENTED);
}

void vm_cmp(vm_t* vm, const REGISTER dest, const uint32_t arg){
    uint32_t destVal = vm->registers[dest];
    uint32_t res = vm->registers[dest] -= arg;
    if(res >> 31)
        vm_set_flag(vm, NF);
    else
        vm_clear_flag(vm, NF);
    if(arg > destVal)
        vm_set_flag(vm, CF);
    else
        vm_clear_flag(vm, CF);
    if(destVal >> 31  != arg >> 31 && arg >> 31 == res >> 31)
        vm_set_flag(vm, OF);
    else
        vm_clear_flag(vm, OF);
    if(res == 0)
        vm_set_flag(vm, ZF);
    else
        vm_clear_flag(vm, ZF);
}

void vm_jmp(vm_t* vm, const uint32_t address, const uint8_t cond){
    if(cond)
        vm->registers[PC] = address;
}

void vm_not(vm_t* vm, const REGISTER dest){
    ~vm->registers[dest];
}

void vm_and(vm_t* vm, const REGISTER dest, const uint32_t arg){
    uint32_t res = vm->registers[dest] &= vm->registers[dest];

    if(res >> 31)
        vm_set_flag(vm, NF);
    else
        vm_clear_flag(vm, NF);
    if(res == 0)
        vm_set_flag(vm, ZF);
    else
        vm_clear_flag(vm, ZF);
    vm_clear_flag(vm, CF);
    vm_clear_flag(vm, OF);
}

void vm_or(vm_t* vm, const REGISTER dest, const uint32_t arg){
    uint32_t res = vm->registers[dest] |= vm->registers[dest];

    if(res >> 31)
        vm_set_flag(vm, NF);
    else
        vm_clear_flag(vm, NF);
    if(res == 0)
        vm_set_flag(vm, ZF);
    else
        vm_clear_flag(vm, ZF);
    vm_clear_flag(vm, CF);
    vm_clear_flag(vm, OF);
}

void vm_xor(vm_t* vm, const REGISTER dest, const uint32_t arg){
    uint32_t res = vm->registers[dest] ^= vm->registers[dest];

    if(res >> 31)
        vm_set_flag(vm, NF);
    else
        vm_clear_flag(vm, NF);
    if(res == 0)
        vm_set_flag(vm, ZF);
    else
        vm_clear_flag(vm, ZF);
    vm_clear_flag(vm, CF);
    vm_clear_flag(vm, OF);
}

void vm_ri_op(vm_t* vm, uint32_t instruction, void (*ri_func)(vm_t*, const REGISTER, const uint32_t)){
    const uint32_t val = instruction & 0xfffff;
    const REGISTER dest = (instruction >> 20) & 0xf;
    ri_func(vm, dest, val);
}
void vm_rl_op(vm_t* vm, uint32_t instruction, void (*rl_func)(vm_t*, const REGISTER, const uint32_t)){
    const uint32_t val = vm->memory[vm->registers[PC]++];
    const REGISTER dest = (instruction >> 20) & 0xf;
    rl_func(vm, dest, val);
}
void vm_rr_op(vm_t* vm, uint32_t instruction, void (*rr_func)(vm_t*, const REGISTER, const uint32_t)){
    const REGISTER dest_reg = (instruction >> 20) & 0xf;
    const uint32_t val = vm->registers[instruction & 0xf];
    rr_func(vm, dest_reg, val);
}
