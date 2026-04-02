#include "memory_s.h"
#include "logging.h"

void* malloc_s(size_t size)
{
    void* result = malloc(size);
    if(!result)
    {
        logErrorM("Failed to allocate memory by calling malloc function. Size: %u", size);
        exit(1);
    }
    return result;
}

void* calloc_s(size_t count, size_t size)
{
    void* result = calloc(count, size);
    if(!result)
    {
        logErrorM("Failed to allocate memory by calling calloc function. Count: %u, Size: %u", count, size);
        exit(1);
    }
    return result;
}

void* realloc_s(void* ptr, size_t size)
{
    void* result = realloc(ptr, size);
    if(!result)
    {
        logErrorM("Failed to allocate memory by calling realloc function. Pointer: %p, Size: %u", ptr, size);
        exit(1);
    }
    return result;
}