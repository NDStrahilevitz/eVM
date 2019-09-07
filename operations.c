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

void vm_pop(vm_t* vm, uint32_t* dest){
    uint32_t sp_loc = vm->registers[SP]--;
    uint32_t val = vm->memory[sp_loc];
    vm->memory[sp_loc] = 0;
    *dest = val;
}

void vm_mov(vm_t* vm, uint32_t* dest, const uint32_t* src){
    *dest = *src;
}

void vm_add(vm_t* vm, uint32_t* dest, const uint32_t* arg){
    uint32_t prevVal = *dest;
    *dest += *arg;
    if(*dest >> 31)
        vm_set_flag(vm, NF);
    else
        vm_clear_flag(vm, NF);
    if(*dest < prevVal)
        vm_set_flag(vm, CF);
    else
        vm_clear_flag(vm, CF);
    if(prevVal >> 31  == *arg >> 31 && *dest >> 31 != prevVal >> 31)
        vm_set_flag(vm, OF);
    else
        vm_clear_flag(vm, OF);
    if(*dest == 0)
        vm_set_flag(vm, ZF);
    else
        vm_clear_flag(vm, ZF);
}

void vm_sub(vm_t* vm, uint32_t* dest, const uint32_t* arg){
    uint32_t prevVal = *dest;
    *dest -= *arg;
    if(*dest >> 31)
        vm_set_flag(vm, NF);
    else
        vm_clear_flag(vm, NF);
    if(*arg > prevVal)
        vm_set_flag(vm, CF);
    else
        vm_clear_flag(vm, CF);
    if(prevVal >> 31  != *arg >> 31 && *arg >> 31 == *dest >> 31)
        vm_set_flag(vm, OF);
    else
        vm_clear_flag(vm, OF);
    if(*dest == 0)
        vm_set_flag(vm, ZF);
    else
        vm_clear_flag(vm, ZF);
}

void vm_mul(vm_t* vm, uint32_t* dest, const uint32_t* arg){
    exit(NOT_YET_IMPLEMENTED);
}

void vm_imul(vm_t* vm, uint32_t* dest, const uint32_t* arg){
    exit(NOT_YET_IMPLEMENTED);
}

void vm_div(vm_t* vm, uint32_t* dest, const uint32_t* arg){
    exit(NOT_YET_IMPLEMENTED);
}

void vm_idiv(vm_t* vm, uint32_t* dest, const uint32_t* arg){
    exit(NOT_YET_IMPLEMENTED);
}

void vm_cmp(vm_t* vm, const uint32_t* arg1, const uint32_t* arg2){
    uint32_t res = *arg1 - *arg2;
    if(res >> 31)
        vm_set_flag(vm, NF);
    else
        vm_clear_flag(vm, NF);
    if(*arg2 > *arg1)
        vm_set_flag(vm, CF);
    else
        vm_clear_flag(vm, CF);
    if(*arg1 >> 31  != *arg2 >> 31 && *arg2 >> 31 == res >> 31)
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
    else
        ++vm->registers[PC];
}

void vm_not(vm_t* vm, uint32_t* dest){
    exit(NOT_YET_IMPLEMENTED);
}

void vm_and(vm_t* vm, uint32_t* dest, const uint32_t* arg){
    *dest &= *arg;

    if(*dest >> 31)
        vm_set_flag(vm, NF);
    else
        vm_clear_flag(vm, NF);
    if(*dest == 0)
        vm_set_flag(vm, ZF);
    else
        vm_clear_flag(vm, ZF);
    vm_clear_flag(vm, CF);
    vm_clear_flag(vm, OF);
}

void vm_or(vm_t* vm, uint32_t* dest, const uint32_t* arg){
    *dest |= *arg;

    if(*dest >> 31)
        vm_set_flag(vm, NF);
    else
        vm_clear_flag(vm, NF);
    if(*dest == 0)
        vm_set_flag(vm, ZF);
    else
        vm_clear_flag(vm, ZF);
    vm_clear_flag(vm, CF);
    vm_clear_flag(vm, OF);
}

void vm_xor(vm_t* vm, uint32_t* dest, const uint32_t* arg){
    *dest ^= *arg;

    if(*dest >> 31)
        vm_set_flag(vm, NF);
    else
        vm_clear_flag(vm, NF);
    if(*dest == 0)
        vm_set_flag(vm, ZF);
    else
        vm_clear_flag(vm, ZF);
    vm_clear_flag(vm, CF);
    vm_clear_flag(vm, OF);
}

void vm_ri_op(vm_t* vm, uint32_t instruction, void (*ri_func)(vm_t*, uint32_t*, const uint32_t*)){
    uint32_t val = instruction & 0xfffff;
    uint32_t* reg = &vm->registers[instruction >> 20 & 0xf];
    ri_func(vm, reg, &val);
}
void vm_rl_op(vm_t* vm, uint32_t instruction, void (*rl_func)(vm_t*, uint32_t*, const uint32_t*)){
    uint32_t* reg = &vm->registers[instruction >> 20 & 0xf];
    uint32_t* val = &vm->memory[vm->registers[PC]++];
    rl_func(vm, reg, val);
}
void vm_rm_op(vm_t* vm, uint32_t instruction, void (*rm_func)(vm_t*, uint32_t*, const uint32_t*)){
    uint32_t* reg = &vm->registers[instruction >> 20 & 0xf];
    uint32_t* address = &vm->memory[instruction & 0xffff];
    rm_func(vm, reg, address);
}
void vm_rr_op(vm_t* vm, uint32_t instruction, void (*rr_func)(vm_t*, uint32_t*, const uint32_t*)){
    uint32_t* dest_reg = &vm->registers[instruction >> 20 & 0xf];
    uint32_t* src_reg = &vm->registers[instruction & 0xf];
    rr_func(vm, dest_reg, src_reg);
}
void vm_ml_op(vm_t* vm, uint32_t instruction, void (*ml_func)(vm_t*, uint32_t*, const uint32_t*)){
    uint32_t* address = &vm->memory[instruction & 0xffff];
    uint32_t* val = &vm->memory[vm->registers[PC]++];
    ml_func(vm, address, val);
}
void vm_mm_op(vm_t* vm, uint32_t instruction, void (*mm_func)(vm_t*, uint32_t*, const uint32_t*)){
    uint32_t* dest_addr = &vm->memory[instruction & 0xffff];
    uint32_t* src_addr = &vm->memory[vm->memory[vm->registers[PC]++]];
    mm_func(vm, dest_addr, src_addr);
}
void vm_mr_op(vm_t* vm, uint32_t instruction, void (*mr_func)(vm_t*, uint32_t*, const uint32_t*)){
    uint32_t* reg = &vm->registers[instruction >> 20 & 0xf];
    uint32_t* address = &vm->memory[instruction & 0xffff];
    mr_func(vm, address, reg);
}
