#ifndef OPERATIONS_H_
#define OPERATIONS_H_

#include "vm.h"

void vm_end(vm_t* vm);
void vm_nop(vm_t* vm);
void vm_push(vm_t* vm, const uint32_t val);
void vm_pop(vm_t* vm, uint32_t* dest);
void vm_mov(uint32_t* dest, const uint32_t* src);
void vm_add(vm_t* vm, uint32_t* dest, const uint32_t* src);
void vm_sub(vm_t* vm, uint32_t* dest, const uint32_t* src);
void vm_mul(vm_t* vm, uint32_t* dest, const uint32_t* src);
void vm_imul(vm_t* vm, uint32_t* dest, const uint32_t* src);
void vm_div(vm_t* vm, uint32_t* dest, const uint32_t* src);
void vm_idiv(vm_t* vm, uint32_t* dest, const uint32_t* src);
void vm_cmp(vm_t* vm, const uint32_t* arg1, const uint32_t* arg2);
void vm_jmp(vm_t* vm, const uint32_t address, const uint8_t do_jump);
void vm_and(vm_t* vm, uint32_t* dest, const uint32_t* arg);
void vm_or(vm_t* vm, uint32_t* dest, const uint32_t* arg);
void vm_xor(vm_t* vm, uint32_t* dest, const uint32_t* arg);

#endif