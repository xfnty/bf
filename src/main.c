#include <stdio.h>
#include <stdlib.h>

#include "bf.h"
#include "util.h"
#include "common.h"


int main(int argc, const char *argv[]) {
    if (argc <= 1) {
        LOG("Usage: bf <input file>");
        return 0;
    }

    if (!file_exists(argv[1])) {
        LOGF_ERROR("file \"%s\" does not exists", argv[1]);
        return 1;
    }

    size_t file_size = 0;
    unsigned char* file_contents = file_load(argv[1], &file_size);
    if (file_size == 0) {
        LOGF_ERROR("could not load \"%s\"", argv[1]);
        return 2;
    }

    bf_vm_t vm = {0};
    if (!bf_vm_init(&vm, (char*)file_contents, file_size, 10))
        goto exit;

    while (bf_vm_tick(&vm));

    exit:
    file_unload(file_contents);
    bf_vm_destroy(&vm);
    return 0;
}
