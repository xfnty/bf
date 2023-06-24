#include "bf.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"


/* types */
typedef bool (*operator_f)(bf_vm_t* vm);


/* function declarations */
bool bf_vm_validate(bf_vm_t* vm);
bool op_next(bf_vm_t* vm);
bool op_prev(bf_vm_t* vm);
bool op_inc(bf_vm_t* vm);
bool op_dec(bf_vm_t* vm);
bool op_put(bf_vm_t* vm);
bool op_get(bf_vm_t* vm);
bool op_bopen(bf_vm_t* vm);
bool op_bclose(bf_vm_t* vm);


/* variables */
static const operator_f operators[256] = {
    ['>'] = op_next,
    ['<'] = op_prev,
    ['+'] = op_inc,
    ['-'] = op_dec,
    ['.'] = op_put,
    [','] = op_get,
    ['['] = op_bopen,
    [']'] = op_bclose,
};


/* function definitions */
bool bf_vm_init(bf_vm_t* vm, char* program, size_t program_size, size_t memory_size) {
    assert(vm != NULL);
    assert(program != NULL);
    assert(program_size > 0);
    assert(memory_size > 0);

    bf_vm_destroy(vm);

    vm->program = malloc(program_size);
    vm->program_size = program_size;
    memcpy(vm->program, program, program_size);

    vm->memory = malloc(memory_size);
    vm->memory_size = memory_size;
    memset(vm->memory, 0, memory_size);

    bf_vm_reset(vm);

    return bf_vm_validate(vm);
}

void bf_vm_reset(bf_vm_t* vm) {
    assert(vm != NULL);
    
    vm->PC = 0;
    vm->MP = 0;
    vm->LD = 0;
    memset(vm->memory, 0, vm->memory_size);
}

bool bf_vm_tick(bf_vm_t* vm) {
    assert(vm != NULL);
    
    if (vm->PC >= vm->program_size)
        return false;

    return operators[vm->program[vm->PC]](vm);
}

void bf_vm_destroy(bf_vm_t* vm) {
    assert(vm != NULL);

    if (vm->memory) free(vm->memory);
    if (vm->program) free(vm->program);

    memset(vm, 0, sizeof(*vm));
}

bool bf_vm_validate(bf_vm_t* vm) {
    int ld = 0;
    for (int i = 0; i < vm->program_size; i++) {
        if (operators[vm->program[i]] == NULL) {
            LOGF_ERROR("Unknown operator '%c'", vm->program[i]);
            return false;
        }

        ld += (vm->program[i] == '[' ? 1 : (vm->program[i] == ']' ? -1 : 0));
        if (ld < 0) {
            LOGF_ERROR("Unmatched close brace at %d", i);
            return false;
        }
    }

    if (ld != 0) {
        LOG_ERROR("Braces does not match");
        return false;
    }

    return true;
}

bool op_next(bf_vm_t* vm) {
    assert(vm != NULL);
    vm->MP = WRAP(vm->MP + 1, 0, vm->memory_size);
    vm->PC++;
    return true;
}

bool op_prev(bf_vm_t* vm) {
    assert(vm != NULL);
    vm->MP = WRAP(vm->MP - 1, 0, vm->memory_size);
    vm->PC++;
    return true;
}

bool op_inc(bf_vm_t* vm) {
    assert(vm != NULL);
    vm->memory[vm->MP]++;
    vm->PC++;
    return true;
}

bool op_dec(bf_vm_t* vm) {
    assert(vm != NULL);
    vm->memory[vm->MP]--;
    vm->PC++;
    return true;
}

bool op_put(bf_vm_t* vm) {
    assert(vm != NULL);
    printf("%c", vm->memory[vm->MP]);
    vm->PC++;
    return true;
}

bool op_get(bf_vm_t* vm) {
    assert(vm != NULL);
    vm->memory[vm->MP] = getchar();
    vm->PC++;
    return true;
}

bool op_bopen(bf_vm_t* vm) {
    assert(vm != NULL);

    if (vm->memory[vm->MP]) {
        vm->PC++;
        return true;
    }

    int pc = vm->PC;
    int ld = vm->LD + 1;
    while (ld != vm->LD && pc < strlen(vm->program)) {
        pc++;
        ld += (vm->program[pc] == '[' ? 1 : (vm->program[pc] == ']' ? -1 : 0));
    }
    
    if (ld != vm->LD) {
        // LOGF_ERROR("Unmatched open bracket at %d", vm->PC);
        return false;
    }

    vm->PC = pc;
    return true;
}

bool op_bclose(bf_vm_t* vm) {
    if (vm->memory[vm->MP] == 0) {
        vm->PC++;
        return true;
    }

    int pc = vm->PC;
    int ld = vm->LD - 1;
    while (ld != vm->LD && pc >= 0) {
        pc--;
        ld += (vm->program[pc] == '[' ? 1 : (vm->program[pc] == ']' ? -1 : 0));
    }
    
    if (ld != vm->LD) {
        // LOGF_ERROR("Unmatched close bracket at %d", vm->PC);
        return false;
    }

    vm->PC = pc;
    return true;
}
