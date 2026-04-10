#ifndef __LIST_H__
#define __LIST_H__

#include "short_types.h"

typedef struct List {
    void* data;
    u32 elementSize;
    u32 count;
    u32 capacity;
} List;

List List_Create(u32 elementSize);
void List_Free(List* list);
void* List_Append(List* list, const void* element);
void List_Remove(List* list, const void* element);
void* List_Get(const List* list, u32 idx);


#endif // __LIST_H__