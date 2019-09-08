#include "vm.h"
#include "stdio.h"


int main(){
    vm_t* vm = vm_create();
    vm_set_flag(vm, CF);
    vm_set_flag(vm, ZF);
    vm_set_flag(vm, OF);
    vm_set_flag(vm, NF);
    print_vm_details(vm);
}