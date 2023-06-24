#include "util.h"

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>

#include "common.h"


bool file_exists(const char* path) {
    assert(path != NULL);

    struct stat statbuf;
    return stat(path, &statbuf) == 0;
}

unsigned char* file_load(const char* path, size_t* size) {
    assert(path != NULL);
    assert(size != NULL);

    DIR *dir = opendir(path);

    if (dir != NULL)
    {
        *size = 0;
        closedir(dir);
        return NULL;
    }

    FILE* file = fopen(path, "rb");
    if (!file) {
        *size = 0;
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    *size = ftell(file);
    fseek(file, 0, SEEK_SET);

    if (*size == 0)
        return NULL;

    unsigned char* buffer = NULL;
    buffer = malloc(*size);
    assert(buffer != NULL);

    if (fread(buffer, 1, *size, file) == 0) {
        free(buffer);
        *size = 0;
        return NULL;
    }

    return buffer;
}

void file_unload(unsigned char* file) {
    assert(file != NULL);

    free(file);
}
