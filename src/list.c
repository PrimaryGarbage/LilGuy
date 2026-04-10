#include "list.h"
#include "result.h"
#include <stdlib.h>
#include <string.h>

static const u32 c_initialCapacity = 4u;

List List_Create(u32 elementSize)
{
    void* data = malloc(c_initialCapacity * elementSize);
    if (!data) MEMORY_PANIC();

    return (List){
        .data = data,
        .capacity = c_initialCapacity,
        .count = 0u,
        .elementSize = elementSize
    };
}

void List_Free(List* list)
{
    free(list->data);
    list->capacity = 0u;
    list->count = 0u;
}

void* List_Append(List* list, const void* element)
{
    if (list->count == list->capacity)
    {
        u32 newCapacity = list->capacity * 2u;
        void* newData = realloc(list->data, newCapacity * list->elementSize);
        if (!newData) MEMORY_PANIC();

        list->data = newData;
        list->capacity = newCapacity;
    }

    memcpy_s(list->data + list->elementSize * list->count, list->elementSize, element, list->elementSize);
    list->count++;

    return list->data + (list->count - 1u) * list->elementSize;
}

void List_Remove(List* list, const void* element)
{
    for (u32 i = 0u; i < list->count; ++i)
    {
        void* ptr = list->data + i * list->elementSize;
        if (ptr == element)
        {
            size_t bufferSize = list->elementSize * (list->count - i - 1);
            void* buffer = malloc(bufferSize);
            if (!buffer) MEMORY_PANIC();

            memcpy_s(buffer, bufferSize, (ptr + list->elementSize), bufferSize);
            memcpy_s(ptr, bufferSize, buffer, bufferSize);
            free(buffer);

            list->count--;
            break;
        }
    }
}

void* List_Get(const List* list, u32 idx)
{
    return list->data + list->elementSize * idx;
}