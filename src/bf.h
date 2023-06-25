#ifndef BRAINFUCK_H_
#define BRAINFUCK_H_

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>


typedef struct {
    char*           program;
    size_t          program_size;
    unsigned char*  memory;
    size_t          memory_size;
    unsigned int    PC;
    unsigned int    MP;
    int             LD;
    unsigned char   ST;
} bf_vm_t;


bool bf_vm_init(bf_vm_t* vm, char* program, size_t program_size, size_t memory_size);
void bf_vm_reset(bf_vm_t* vm);
bool bf_vm_tick(bf_vm_t* vm);
void bf_vm_destroy(bf_vm_t* vm);

#endif
