#ifndef OPERATIONS_H_
#define OPERATIONS_H_

#include "vm.h"

void vm_end(vm_t* vm);
void vm_nop(vm_t* vm);
void vm_push(vm_t* vm, const uint32_t val);
void vm_pop(vm_t* vm, const REGISTER dest);
void vm_mov(vm_t* vm, const REGISTER dest, const uint32_t arg);
void vm_ldr(vm_t* vm, const REGISTER dest, const REGISTER ptr_reg);
void vm_str(vm_t* vm, const REGISTER ptr_reg, const REGISTER val_reg);
void vm_add(vm_t* vm, const REGISTER dest, const uint32_t arg);
void vm_sub(vm_t* vm, const REGISTER dest, const uint32_t arg);
void vm_mul(vm_t* vm, const REGISTER dest, const uint32_t arg);
void vm_imul(vm_t* vm, const REGISTER dest, const uint32_t arg);
void vm_div(vm_t* vm, const REGISTER dest, const uint32_t arg);
void vm_idiv(vm_t* vm, const REGISTER dest, const uint32_t arg);
void vm_cmp(vm_t* vm, const REGISTER dest, const uint32_t arg);
void vm_jmp(vm_t* vm, const uint32_t address, const uint8_t cond);
void vm_not(vm_t* vm, const REGISTER dest);
void vm_and(vm_t* vm, const REGISTER dest, const uint32_t arg);
void vm_or(vm_t* vm, const REGISTER dest, const uint32_t arg);
void vm_xor(vm_t* vm, const REGISTER dest, const uint32_t arg);

void vm_ri_op(vm_t* vm, uint32_t instruction, void (*ri_func)(vm_t*, const REGISTER, const uint32_t));
void vm_rl_op(vm_t* vm, uint32_t instruction, void (*rl_func)(vm_t*, const REGISTER, const uint32_t));
void vm_rr_op(vm_t* vm, uint32_t instruction, void (*rr_func)(vm_t*, const REGISTER, const uint32_t));

#endif