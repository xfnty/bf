#include <stdio.h>
#include <dirent.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/stat.h>

#define LENGTH(array) (sizeof(array)/sizeof(array[0]))
#define CONSTRAIN(v, a, b) ((v < a) ? a : ((v > b) ? b : v))
#define WRAP(v, a, b) ((v < a) ? b : ((v > b) ? a : v))
#define MIN(a, b) ((a < b) ? a : b)
#define MAX(a, b) ((a > b) ? a : b)
#define ANSI_RESET      "\033[0;97m"
#define ANSI_RED_BOLD   "\033[1;91m"
#define LOG(fmt, ...) printf(ANSI_RESET fmt ANSI_RESET "\n", ##__VA_ARGS__)
#define LOG_ERROR(fmt, ...) printf(ANSI_RED_BOLD "error: " ANSI_RESET fmt ANSI_RESET "\n", ##__VA_ARGS__)
#define ASSERT_LOG_RETURN(cond, value, fmt, ...) do { if (!(cond)) { LOG(fmt, ##__VA_ARGS__); return (value); } } while(0)
#define ASSERT_LOG_ERROR_RETURN(cond, value, fmt, ...) do { if (!(cond)) { LOG_ERROR(fmt, ##__VA_ARGS__); return (value); } } while(0)


bool file_exists(const char* path);
bool file_load(const char* path, char** buffer, int* size);
void file_unload(char** buffer);

int main(int argc, const char *argv[]) {
    ASSERT_LOG_RETURN(argc > 1, 0, "Usage: bf <file>");

    const char* script_path = argv[1];
    ASSERT_LOG_ERROR_RETURN(file_exists(script_path), 1, "file \"%s\" does not exists.", script_path);

    char* program = NULL;
    int prorgam_size = 0;
    ASSERT_LOG_ERROR_RETURN(file_load(script_path, &program, &prorgam_size), 2, "Could not load file \"%s\".", script_path);

    unsigned int    PC = 0;  // Program Counter
    unsigned int    MP = 0;  // Memory Pointer
    int             LD = 0;  // Loop Depth
    unsigned char   memory[30000] = {0};
    while (PC < prorgam_size) {
        char op = program[PC];

        int pc, ld;
        switch (op) {
        case '<':
            MP = WRAP(MP - 1, 0, prorgam_size - 1);
            PC++;
            break;

        case '>':
            MP = WRAP(MP + 1, 0, prorgam_size - 1);
            PC++;
            break;

        case '+':
            memory[MP]++;
            PC++;
            break;

        case '-':
            memory[MP]--;
            PC++;
            break;

        case '.':
            printf("%c", memory[MP]);
            PC++;
            break;

        case ',':
            memory[MP] = getchar();
            PC++;
            break;

        case '[':
            if (memory[MP]) {
                LD++;
                PC++;
                break;
            }

            pc = PC;
            ld = LD + 1;
            while (ld != LD && pc < prorgam_size) {
                pc++;
                ld += (program[pc] == '[' ? 1 : (program[pc] == ']' ? -1 : 0));
            }
            ASSERT_LOG_ERROR_RETURN(ld == LD, 3, "Unmatched open bracket at %d", PC);

            PC = pc;
            break;

        case ']':
            if (memory[MP] == 0) {
                LD--;
                PC++;
                break;
            }

            int pc = PC;
            int ld = LD - 1;
            while (ld != LD && pc >= 0) {
                pc--;
                ld += (program[pc] == '[' ? 1 : (program[pc] == ']' ? -1 : 0));
            }
            ASSERT_LOG_ERROR_RETURN(ld == LD, 3, "Unmatched close bracket at %d", PC);

            PC = pc;
            break;

        default:
            // LOG_ERROR("Unknown opcode '%c' (%d) at %d", op, (int)op, PC);
            // return 4;
            PC++;
            break;
        }
    }

    file_unload(&program);
    return 0;
}


bool file_exists(const char* path) {
    assert(path != NULL);

    struct stat statbuf;
    return stat(path, &statbuf) == 0;
}

bool file_load(const char* path, char** buffer, int* size) {
    assert(path != NULL);

    int _s = 0;
    if (size == NULL)
        size = &_s;

    *buffer = NULL;
    *size = 0;

    DIR *dir = opendir(path);
    if (dir != NULL) {
        closedir(dir);
        return false;
    }

    FILE* file = fopen(path, "rb");
    if (!file) {
        return false;
    }

    fseek(file, 0, SEEK_END);
    *size = ftell(file);
    fseek(file, 0, SEEK_SET);

    if (*size == 0)
        return false;

    *buffer = malloc(*size);
    assert(*buffer != NULL);

    if (fread(*buffer, 1, *size, file) == 0) {
        free(*buffer);
        *size = 0;
        return false;
    }

    return true;
}

void file_unload(char** buffer) {
    assert(buffer != NULL);

    if (*buffer) {
        free(*buffer);
        *buffer = NULL;
    }
}
