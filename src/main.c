#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <signal.h>

#include <kvec.h>

#include "common.h"
#include "util.h"

#define SILENT


void signal_handler(int sig);

typedef struct {
    const char* program;
    unsigned char array[5];
    int PC;  // program counter
    int AP;  // array pointer
    int LD;  // loop depth
    int out; // output character
} interpreter_t;

typedef bool (*operator_f)(interpreter_t* ip);

bool op_next(interpreter_t* ip) { ip->AP = WRAP(ip->AP + 1, 0, ARRAY_LENGTH(ip->array));    ip->PC++; return true; }
bool op_prev(interpreter_t* ip) { ip->AP = WRAP(ip->AP - 1, 0, ARRAY_LENGTH(ip->array));    ip->PC++; return true; }
bool op_inc(interpreter_t* ip)  { ip->array[ip->AP]++;                                      ip->PC++; return true; }
bool op_dec(interpreter_t* ip)  { ip->array[ip->AP]--;                                      ip->PC++; return true; }
// bool op_put(interpreter_t* ip)  { printf("%d\n", (int)ip->array[ip->AP]);                   ip->PC++; return true; }
bool op_put(interpreter_t* ip)  { ip->out = ip->array[ip->AP];                              ip->PC++; return true; }
bool op_get(interpreter_t* ip)  { ip->array[ip->AP] = getchar();                            ip->PC++; return true; }
bool op_bopen(interpreter_t* ip) {
    if (ip->array[ip->AP]) {
        ip->PC++;
        return true;
    }

    int pc = ip->PC;
    int ld = ip->LD + 1;
    while (ld != ip->LD && pc < strlen(ip->program)) {
        pc++;
        ld += (ip->program[pc] == '[' ? 1 : (ip->program[pc] == ']' ? -1 : 0));
    }
    
    if (ld != ip->LD) {
        LOGF_ERROR("Unmatched open bracket at %d", ip->PC);
        return false;
    }

    ip->PC = pc;
    return true;
}
bool op_bclose(interpreter_t* ip) {
    if (ip->array[ip->AP] == 0) {
        ip->PC++;
        return true;
    }

    int pc = ip->PC;
    int ld = ip->LD - 1;
    while (ld != ip->LD && pc >= 0) {
        pc--;
        ld += (ip->program[pc] == '[' ? 1 : (ip->program[pc] == ']' ? -1 : 0));
    }
    
    if (ld != ip->LD) {
        LOGF_ERROR("Unmatched close bracket at %d", ip->PC);
        return false;
    }

    ip->PC = pc;
    return true;
}

int main(int argc, const char *argv[]) {
    signal(SIGINT, signal_handler);

    const operator_f operators[256] = {
        ['>'] = op_next,
        ['<'] = op_prev,
        ['+'] = op_inc,
        ['-'] = op_dec,
        ['.'] = op_put,
        [','] = op_get,
        ['['] = op_bopen,
        [']'] = op_bclose,
    };

    // interpreter_t ip = { "+++>++<[->+<]>.", {0}, 0, 0, 0, -1 };  // 5
    interpreter_t ip = { "++++++++++[>+++++++>++++++++++>+++>++++<<<<-]>++.>+.+++++++..+++.>>++++.<++.<++++++++.--------.+++.------.--------.>+.", {0}, 0, 0, 0, -1 };  // 5
    // interpreter_t ip = { "++++[.-].", {0}, 0, 0, 0, -1 };  // 43210

    for (; ip.PC < strlen(ip.program);) {
        char c = ip.program[ip.PC];
        
        if (operators[c] == NULL) {
            LOGF(ANSI_RED "Unknown operator '%c'", c);
            break;
        }

#ifndef SILENT
        for (int i = 0; i < strlen(ip.program); i++)
            printf("%s%c" ANSI_GRAY, (i == ip.PC) ? ANSI_WHITE_BOLD : ANSI_GRAY, ip.program[i]);
        printf(ANSI_RESET "  ");
        for (int i = 0; i < ARRAY_LENGTH(ip.array); i++)
            printf("%s%4d" ANSI_GRAY, (i == ip.AP) ? ANSI_WHITE_BOLD : ANSI_GRAY, (int)ip.array[i]);
        printf(ANSI_RESET "    => ");
#endif

        if (!operators[c](&ip))
            break;

#ifndef SILENT  
        for (int i = 0; i < ARRAY_LENGTH(ip.array); i++)
            printf("%s%4d" ANSI_GRAY, (i == ip.AP) ? ANSI_WHITE_BOLD : ANSI_GRAY, (int)ip.array[i]);

        if (ip.out > -1)
            printf(ANSI_RESET "   : %d\n", ip.out);
        else
            printf(ANSI_RESET "\n");
#else
        if (ip.out > -1)
            printf(ANSI_RESET "%c", ip.out);
#endif

        ip.out = -1;
    }

    printf("\n");
    return 0;
}

void signal_handler(int sig) {
    printf("\n");
    exit(0);
}