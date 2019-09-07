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

void vm_mov(uint32_t* dest, const uint32_t* src){
    *dest = *src;
}

void vm_add(vm_t* vm, uint32_t* dest, const uint32_t* src){
    uint32_t prevVal = *dest;
    *dest += *src;
    if(*dest >> 31)
        vm_set_flag(vm, NF);
    else
        vm_clear_flag(vm, NF);
    if(*dest < prevVal)
        vm_set_flag(vm, CF);
    else
        vm_clear_flag(vm, CF);
    if(prevVal >> 31  == *src >> 31 && *dest >> 31 != prevVal >> 31)
        vm_set_flag(vm, OF);
    else
        vm_clear_flag(vm, OF);
    if(*dest == 0)
        vm_set_flag(vm, ZF);
    else
        vm_clear_flag(vm, ZF);
}

void vm_sub(vm_t* vm, uint32_t* dest, const uint32_t* src){
    uint32_t prevVal = *dest;
    *dest -= *src;
    if(*dest >> 31)
        vm_set_flag(vm, NF);
    else
        vm_clear_flag(vm, NF);
    if(*dest > prevVal)
        vm_set_flag(vm, CF);
    else
        vm_clear_flag(vm, CF);
    if(prevVal >> 31  == *src >> 31 && *dest >> 31 != prevVal >> 31)
        vm_set_flag(vm, OF);
    else
        vm_clear_flag(vm, OF);
    if(*dest == 0)
        vm_set_flag(vm, ZF);
    else
        vm_clear_flag(vm, ZF);
}

void vm_mul(vm_t* vm, uint32_t* dest, const uint32_t* src){}

void vm_imul(vm_t* vm, uint32_t* dest, const uint32_t* src){}
void vm_div(vm_t* vm, uint32_t* dest, const uint32_t* src){}
void vm_idiv(vm_t* vm, uint32_t* dest, const uint32_t* src){}

void vm_cmp(vm_t* vm, const uint32_t* arg1, const uint32_t* arg2){
    uint32_t res = *arg1 - *arg2;
    if(res >> 31)
        vm_set_flag(vm, NF);
    else
        vm_clear_flag(vm, NF);
    if(res > *arg1)
        vm_set_flag(vm, CF);
    else
        vm_clear_flag(vm, CF);
    if(*arg1 >> 31  == *arg2 >> 31 && *arg1 >> 31 != res >> 31)
        vm_set_flag(vm, OF);
    else
        vm_clear_flag(vm, OF);
    if(res == 0)
        vm_set_flag(vm, ZF);
    else
        vm_clear_flag(vm, ZF);
}

void vm_jmp(vm_t* vm, const uint32_t address, const uint8_t do_jump){
    if(do_jump)
        vm->memory[PC] = address;
    else
        ++vm->memory[PC];
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
