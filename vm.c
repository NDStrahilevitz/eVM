#include "vm.h"
#include "operations.h"
#include "exitcodes.h"

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
    uint32_t instruction = vm->memory[vm->registers[PC]++];
    uint32_t opcode = instruction >> 24 & 0xff;

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

        case PUSHM:{
            uint32_t address = instruction & 0xffff;
            uint32_t val = vm->memory[address];
            vm_push(vm, val);
            break;
        }

        case PUSHR:{
            uint32_t reg = instruction >> 20 & 0xf;
            uint32_t val = vm->registers[reg];
            vm_push(vm, val);
            break;
        }

        case POPM:{
            uint32_t* address = &vm->memory[instruction & 0xffff];
            vm_pop(vm, address);
            break;
        }

        case POPR:{
            uint32_t* reg = &vm->registers[instruction >> 20 & 0xf];
            vm_pop(vm, reg);
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

        case MOVRM:{
            vm_rm_op(vm, instruction, vm_mov);
            break;
        }

        case MOVRR:{
            vm_rr_op(vm, instruction, vm_mov);
            break;
        }

        case MOVML:{
            vm_ml_op(vm, instruction, vm_mov);
            break;
        }

        case MOVMM:{
            vm_mm_op(vm, instruction, vm_mov);
            break;
        }

        case MOVMR:{
            vm_mr_op(vm, instruction, vm_mov);
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

        case ADDRM:{
            vm_rm_op(vm, instruction, vm_add);
            break;
        }

        case ADDRR:{
            vm_rr_op(vm, instruction, vm_add);
            break;
        }

        case ADDML:{
            vm_ml_op(vm, instruction, vm_add);
            break;
        }

        case ADDMM:{
            vm_mm_op(vm, instruction, vm_add);
            break;
        }

        case ADDMR:{
            vm_mr_op(vm, instruction, vm_add);
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

        case SUBRM:{
            vm_rm_op(vm, instruction, vm_sub);
            break;
        }

        case SUBRR:{
            vm_rr_op(vm, instruction, vm_sub);
            break;
        }

        case SUBML:{
            vm_ml_op(vm, instruction, vm_sub);
            break;
        }

        case SUBMM:{
            vm_mm_op(vm, instruction, vm_sub);
            break;
        }

        case SUBMR:{
            vm_mr_op(vm, instruction, vm_sub);
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

        case MULRM:{
            vm_rm_op(vm, instruction, vm_mul);
            break;
        }

        case MULRR:{
            vm_rr_op(vm, instruction, vm_mul);
            break;
        }

        case MULML:{
            vm_ml_op(vm, instruction, vm_mul);
            break;
        }

        case MULMM:{
            vm_mm_op(vm, instruction, vm_mul);
            break;
        }

        case MULMR:{
            vm_mr_op(vm, instruction, vm_mul);
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

        case DIVRM:{
            vm_rm_op(vm, instruction, vm_div);
            break;
        }

        case DIVRR:{
            vm_rr_op(vm, instruction, vm_div);
            break;
        }

        case DIVML:{
            vm_ml_op(vm, instruction, vm_div);
            break;
        }

        case DIVMM:{
            vm_mm_op(vm, instruction, vm_div);
            break;
        }

        case DIVMR:{
            vm_mr_op(vm, instruction, vm_div);
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

        case IMULRM:{
            vm_rm_op(vm, instruction, vm_imul);
            break;
        }

        case IMULRR:{
            vm_rr_op(vm, instruction, vm_imul);
            break;
        }

        case IMULML:{
            vm_ml_op(vm, instruction, vm_imul);
            break;
        }

        case IMULMM:{
            vm_mm_op(vm, instruction, vm_imul);
            break;
        }

        case IMULMR:{
            vm_mr_op(vm, instruction, vm_imul);
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

        case IDIVRM:{
            vm_rm_op(vm, instruction, vm_idiv);
            break;
        }

        case IDIVRR:{
            vm_rr_op(vm, instruction, vm_idiv);
            break;
        }

        case IDIVML:{
            vm_ml_op(vm, instruction, vm_idiv);
            break;
        }

        case IDIVMM:{
            vm_mm_op(vm, instruction, vm_idiv);
            break;
        }

        case IDIVMR:{
            vm_mr_op(vm, instruction, vm_idiv);
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

         case NOTM:{
            uint32_t* address = &vm->memory[instruction & 0xffff];
            vm_not(vm, address);
            break;
        }

        case NOTR:{
            uint32_t* reg = &vm->registers[instruction >> 20 & 0xf];
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

        case ANDRM:{
            vm_rm_op(vm, instruction, vm_and);
            break;
        }

        case ANDRR:{
            vm_rr_op(vm, instruction, vm_and);
            break;
        }

        case ANDML:{
            vm_ml_op(vm, instruction, vm_and);
            break;
        }

        case ANDMM:{
            vm_mm_op(vm, instruction, vm_and);
            break;
        }

        case ANDMR:{
            vm_mr_op(vm, instruction, vm_and);
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

        case ORRM:{
            vm_rm_op(vm, instruction, vm_or);
            break;
        }

        case ORRR:{
            vm_rr_op(vm, instruction, vm_or);
            break;
        }

        case ORML:{
            vm_ml_op(vm, instruction, vm_or);
            break;
        }

        case ORMM:{
            vm_mm_op(vm, instruction, vm_or);
            break;
        }

        case ORMR:{
            vm_mr_op(vm, instruction, vm_or);
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

        case XORRM:{
            vm_rm_op(vm, instruction, vm_xor);
            break;
        }

        case XORRR:{
            vm_rr_op(vm, instruction, vm_xor);
            break;
        }

        case XORML:{
            vm_ml_op(vm, instruction, vm_xor);
            break;
        }

        case XORMM:{
            vm_mm_op(vm, instruction, vm_xor);
            break;
        }

        case XORMR:{
            vm_mr_op(vm, instruction, vm_xor);
            break;
        }

        default:
            exit(OPCODE_NOT_EXIST);
            break;

    }
}
