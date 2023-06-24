#ifndef UTIL_H_
#define UTIL_H_

#include <stddef.h>
#include <stdbool.h>


bool            file_exists(const char* path);
unsigned char*  file_load(const char* path, size_t* size);
void            file_unload(unsigned char* file);

const char*     humanize_bytesize(size_t value);

#endif  // UTIL_H_
