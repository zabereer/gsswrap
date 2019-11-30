#ifndef GSSWRAP_COMMON_H
#define GSSWRAP_COMMON_H

#include "callbacks.h"

#include <stdbool.h>

struct gsswrap_context;

/**
 * 
 */
struct gsswrap_context* gsswrap_make_context(
    gsswrap_send_token_fn send_function,
    gsswrap_recv_token_fn recv_function,
    void* user_ctx);
void gsswrap_free_context(struct gsswrap_context*);
void* gsswrap_user_data(struct gsswrap_context*);

/**
 * Returns last error string, NULL if none.
 * Ownership of the string remains with gsswrap, do not free it.
 */
const char* gsswrap_last_error(struct gsswrap_context*);

#endif
