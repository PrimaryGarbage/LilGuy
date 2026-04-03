#ifndef __ERROR_H__
#define __ERROR_H__

#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>

#define IF_ERROR(result, code) \
if (IsError(&result)) \
{ \
    do { code } while(0); \
}

#define IF_ERROR_RETURN(result) \
if (IsError(&result)) return result

#define IF_ERROR_PANIC(result) \
if (IsError(&result)) exit(1)

#define IF_ERROR_PANIC_EX(result, code) \
do { \
    if (IsError(&result)) \
    { \
        code \
        exit(1); \
    } \
} while(0)

#define PANIC() \
do { \
    fprintf(stderr, "Program panicked"); \
    exit(1); \
} while(0)

#define MEMORY_PANIC() \
do { \
    fprintf(stderr, "Program panicked after failed memory allocation"); \
} while(0)

#define MEMORY_PANIC_EX(code) \
do { \
    fprintf(stderr, "Program panicked after failed memory allocation"); \
    code; \
} while(0)


typedef enum ResultCode {
    RESULT_SUCCESS = 0,
    RESULT_GENERIC_ERROR,
    RESULT_MEMORY_ALLOCATION_ERROR,
    RESULT_GRAPHICS_CONTEXT_INITIALIZATION_ERROR,
    RESULT_MINIFB_INITIALIZATION_ERROR,
    RESULT_SCREEN_CAPTURE_ERROR,
} ResultCode;

typedef struct Result {
    ResultCode code;
    const char* message;
} Result;

static inline Result Success()
{
    return (Result){ RESULT_SUCCESS, NULL };
}

static inline bool IsError(const Result* result)
{
    return result->code != RESULT_SUCCESS;
}

static inline Result GenericError(const char* message)
{
    return (Result){ RESULT_GENERIC_ERROR, message };
}

static inline Result Error(ResultCode code, const char* message)
{
    return (Result){ code, message };
}

#endif // __ERROR_H__