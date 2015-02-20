#ifndef MWM_ERROR_H
#define MWM_ERROR_H
#include <stdlib.h>

typedef struct {
    const char* err_str;
    int err_code;
    int critical;
} internal_Error;

internal_Error* error_get_last();

#endif /* MWM_ERROR_H */
