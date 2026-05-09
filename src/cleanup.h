#ifndef __CLEANUP_H__
#define __CLEANUP_H__

typedef void (*Cleanup_Callback)(void);

void Cleanup_AddCallback(Cleanup_Callback callback);
void Cleanup_Execute();

#endif // __CLEANUP_H__