#ifndef __TWINE_H__
#define __TWINE_H__

typedef struct Twine* TwineHandle;

typedef void(*TwineFunction)(double elapsed, void* additionalData);
typedef void(*TwineOnFinishCallback)(void);

void Twine_Update(double deltatime);
TwineHandle Twine_CreateFunction(double timeLimit, TwineFunction function, void* additionalData);
void Twine_SetOnFinishCallback(TwineHandle twine, TwineOnFinishCallback callback);

#endif // __TWINE_H__