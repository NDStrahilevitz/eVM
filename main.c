#include "vm.h"


int main(){
    vm_t* vm = vm_create();
    read_program_from_file(vm, "res.evm");
    vm_start(vm);
    vm_run(vm);
    print_vm_details(vm);
    vm_destroy(vm);
}