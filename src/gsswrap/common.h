#ifndef GSSWRAP_COMMON_H
#define GSSWRAP_COMMON_H

/**
 * Functions common to both client and server side.
 */

#include "callbacks.h"

#include <stdbool.h>

struct gsswrap_context;

/**
 * Create gsswrap_context using send_function and recv_function.
 * The user_data value is also stored in the context and returned
 * on the send/recv function invocations.
 */
struct gsswrap_context* gsswrap_make_context(
    gsswrap_send_token_fn send_function,
    gsswrap_recv_token_fn recv_function,
    void* user_data);

/**
 * Destroy the gsswrap_context freeing all associated resources.
 * The user_data value is returned.
 */
void* gsswrap_destroy_context(struct gsswrap_context*);

/**
 * Retrieve user_data stored in context.
 */
void* gsswrap_user_data(struct gsswrap_context*);

/**
 * Reset user_data with new value and return old user_data
 * stored in context.
 */
void* gsswrap_reset_user_data(struct gsswrap_context*, void* user_data);

/**
 * Returns last error string or NULL if none.
 * Ownership of the string remains with gsswrap, do not free it.
 */
const char* gsswrap_last_error(struct gsswrap_context*);

#endif
