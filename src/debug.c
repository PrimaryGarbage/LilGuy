#include "debug.h"
#include <stdio.h>
#include <string.h>

#define PRINT_SCENE_BUFFER_SIZE 512

static void Debug_PrintSceneTreeRecurse(const Scene* scene, u32 indent)
{
    constexpr char indentChar = '-';
    constexpr u32 indentPerChild = 1u;

    char buffer[PRINT_SCENE_BUFFER_SIZE];
    u32 idx = 0u;
    for(; idx < indent; ++idx)
        buffer[idx] = indentChar;

    strcpy_s(buffer + idx, PRINT_SCENE_BUFFER_SIZE - idx, scene->name);
    printf("%s\n", buffer);

    for(u32 i = 0u; i < scene->childrenCount; ++i)
        Debug_PrintSceneTreeRecurse(scene->children[i], indent + indentPerChild);
}

void Debug_PrintSceneTree(const Scene* scene)
{
    Debug_PrintSceneTreeRecurse(scene, 0u);
}
