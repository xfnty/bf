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

const char* humanize_bytesize(size_t value) {
    const char* suffixes[] = { " B", " kB", " MB", " GB", " TB" };
    static char buffer[64] = {0};

    int i = 0;
    while (value > 1024 && i < ARRAY_LENGTH(suffixes) - 1) {
        i++;
        value /= 1024;
    }

    snprintf(buffer, sizeof(buffer), "%lu%s", value, suffixes[i]);
    return buffer;
}
