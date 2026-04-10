#include "twine.h"
#include "logging.h"
#include "result.h"
#include "short_types.h"

#define TWINE_MAX_INSTANCES 100

typedef struct Twine {
    double elapsed;
    double limit;
    TwineFunction function;
    TwineOnFinishCallback onFinish;
    void* additionalData;
    bool active;
} Twine;

static Twine s_twines[TWINE_MAX_INSTANCES];

void Twine_Update(double deltatime)
{
    for(u32 i = 0; i < TWINE_MAX_INSTANCES; ++i)
    {
        Twine* twine = &s_twines[i];
        if (!twine->active) continue;

        twine->elapsed += deltatime;

        if (twine->elapsed > twine->limit)
        {
            if (twine->onFinish)
                twine->onFinish();

            twine->active = false;
            free(twine->additionalData);
        }
        else
        {
            twine->function(twine->elapsed, twine->additionalData);
        }
    }
}

TwineHandle Twine_CreateFunction(double timeLimit, TwineFunction function, void* additionalData)
{
    for(u32 i = 0u; i < TWINE_MAX_INSTANCES; ++i)
    {
        if (!s_twines[i].active)
        {
            s_twines[i] = (Twine){
                .elapsed = 0.0,
                .limit = timeLimit,
                .function = function,
                .additionalData = additionalData,
                .active = true
            };
            
            return &s_twines[i];
        }
    }
    
    PANIC_EX(LogErrorM("Failed to create twine function: Unable to find inactive twine."););
}

void Twine_SetOnFinishCallback(TwineHandle twine, TwineOnFinishCallback callback)
{
    twine->onFinish = callback;
}