#include <stdio.h>
#include <time.h>
#include <stdarg.h>
#include "short_types.h"
#include "logging.h"
#include "result.h"

#define TIME_STRING_SIZE 80

static void localtime_c(struct tm* local_time, time_t* raw_time)
{
#ifdef _WIN32
    localtime_s(local_time, raw_time);
#else
    localtime_r(raw_time, local_time);
#endif
}


void getTimeString(char* str, size_t strSize)
{
    time_t rawTime;
    struct tm localTime;
    time(&rawTime);
    localtime_c(&localTime, &rawTime);
    strftime(str, strSize, "%Y-%m-%d %H:%M:%S", &localTime);
}

void logInfo(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);

    char timeString[TIME_STRING_SIZE];
    getTimeString(timeString, TIME_STRING_SIZE);
    fprintf(stdout, "%s [INFO]: ", timeString);
    vfprintf(stdout, fmt, args);
    fputc('\n', stdout);

    va_end(args);
}

void logWarning(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);

    char timeString[TIME_STRING_SIZE];
    fprintf(stdout, "%s [WARNING]: ", timeString);
    vfprintf(stdout, fmt, args);
    fputc('\n', stdout);

    va_end(args);
}

void logError(const Result* result, const char* optionalMessage)
{
    char timeString[TIME_STRING_SIZE];
    getTimeString(timeString, TIME_STRING_SIZE);
    fprintf(stderr, "%s [ERROR]: Error code %d, error message: %s; %s\n", timeString, (int)result->code, result->message, optionalMessage);
}

void logErrorM(const char* fmt, ...)
{
    va_list args;
    va_start(args, fmt);

    char timeString[TIME_STRING_SIZE];
    getTimeString(timeString, TIME_STRING_SIZE);
    fprintf(stderr, "%s [ERROR]: ", timeString);
    vfprintf(stderr, fmt, args);
    fputc('\n', stderr);

    va_end(args);
}
