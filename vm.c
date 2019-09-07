#include "vm.h"
#include "operations.h"

vm_t* vm_init(){
    vm_t* new_vm = (vm_t*)malloc(sizeof(vm_t*));
    new_vm->running = 1;

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

void vm_run(vm_t* vm){
    while(vm->running)
        vm_cpu_cycle(vm);
}
void vm_cpu_cycle(vm_t* vm){
    uint32_t instruction = vm->memory[PC]++;
    uint32_t opcode = instruction >> 24 & 0xff;

    switch(opcode){
        case END:{
            vm_end(vm);
        }
        
        case NOP:{
            vm_nop(vm);
        }

        case PUSHI:{
            uint32_t val = instruction & 0xfffff;
            vm_push(vm, val);
        }

        case PUSHL:{
            uint32_t val = vm->memory[PC]++;
            vm_push(vm, val);
        }

        case PUSHM:{
            uint32_t address = instruction & 0xffff;
            uint32_t val = vm->memory[address];
            vm_push(vm, val);
        }

        case PUSHR:{
            uint32_t reg = instruction >> 20 & 0xf;
            uint32_t val = vm->registers[reg];
            vm_push(vm, val);
        }
    }
}
