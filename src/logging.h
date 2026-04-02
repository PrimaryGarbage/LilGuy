#ifndef __LOGGING_H__
#define __LOGGING_H__

struct Result;

void logInfo(const char* fmt, ...);
void logWarning(const char* fmt, ...);
void logError(const struct Result* result, const char* optionalMessage);
void logErrorM(const char* fmt, ...);

#endif // __LOGGING_H__