#include <error.h>

static internal_Error *last_error = NULL;

internal_Error* error_get_last() {
    return last_error;
}
