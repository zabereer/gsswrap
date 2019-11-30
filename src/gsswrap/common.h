#ifndef GSSWRAP_COMMON_H
#define GSSWRAP_COMMON_H

/**
 * Functions common to both client and server side.
 */

#include "callbacks.h"

struct gsswrap_context;

/**
 * Create gsswrap_context using send_function and recv_function.
 */
struct gsswrap_context* gsswrap_make_context(
    gsswrap_send_token_fn send_function,
    gsswrap_recv_token_fn recv_function);

/**
 * Destroy the gsswrap_context freeing all associated resources.
 */
void gsswrap_destroy_context(struct gsswrap_context*);

/**
 * Returns last error string or NULL if none.
 * Ownership of the string remains with gsswrap, do not free it.
 */
const char* gsswrap_last_error(struct gsswrap_context*);

#endif
