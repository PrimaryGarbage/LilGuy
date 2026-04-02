#ifndef __LOGGING_H__
#define __LOGGING_H__

struct Result;

void LogInfo(const char* fmt, ...);
void LogWarning(const char* fmt, ...);
void LogError(const struct Result* result, const char* optionalMessage);
void LogErrorM(const char* fmt, ...);

#endif // __LOGGING_H__