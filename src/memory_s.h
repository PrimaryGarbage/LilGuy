#ifndef __MEMORY_S_H__
#define __MEMORY_S_H__

#include "short_types.h"
#include <stdlib.h>

void* malloc_s(size_t size);

void* calloc_s(size_t count, size_t size);

void* realloc_s(void* ptr, size_t size);

#endif // __MEMORY_S_H__