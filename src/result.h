#ifndef __ERROR_H__
#define __ERROR_H__

#include "short_types.h"
#include <stdbool.h>
#include <stdarg.h>

#define IF_ERROR(result, code) \
if (IsError(&result)) \
{ \
    code \
}

#define IF_ERROR_RETURN(result) \
if(IsError(&result)) return result

typedef enum ResultCode {
    RESULT_SUCCESS = 0,
    RESULT_GENERIC_ERROR,
    RESULT_GRAPHICS_CONTEXT_INITIALIZATION_ERROR,
    RESULT_MINIFB_INITIALIZATION_ERROR,
} ResultCode;

typedef struct Result {
    ResultCode code;
    const char* message;
} Result;

static const Result result_success = { RESULT_SUCCESS, NULL };

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