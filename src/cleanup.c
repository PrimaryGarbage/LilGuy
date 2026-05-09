#include "cleanup.h"
#include "result.h"
#include "short_types.h"

#define CLEANUP_CALLBACK_COUNT_MAX 64

static Cleanup_Callback s_callbacks[CLEANUP_CALLBACK_COUNT_MAX];
static u32 s_callbacksCount;

void Cleanup_AddCallback(Cleanup_Callback callback)
{
    if (s_callbacksCount == CLEANUP_CALLBACK_COUNT_MAX)
        PANIC_M("Max number of cleanup callbacks exceeded!");

    s_callbacks[s_callbacksCount++] = callback;
}

void Cleanup_Execute()
{
    for(u32 i = 0u; i < s_callbacksCount; ++i)
        s_callbacks[i]();

    s_callbacksCount = 0u;
}
