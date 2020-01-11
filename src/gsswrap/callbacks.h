#ifndef GSSWRAP_CALLBACKS_H
#define GSSWRAP_CALLBACKS_H

#include <stdbool.h>
#include <stddef.h>

/**
 * Functions used by gsswrap_initiate() and gsswrap_accept() via
 * gsswrap_context to exchange tokens with the peer.
 * gsswrap_send_token_fn() is called to send a token to the peer.
 * gsswrap_recv_token_fn() is called to receive a token from the peer.
 * gsswrap_free_buffer_fn() is called after gsswrap_recv_token_fn()
 * to free the token buffer it returned.
 * The user_data argument will be the same value as was passed to either
 * gsswrap_initiate() and gsswrap_accept().
 */
typedef bool (*gsswrap_send_token_fn)(
    const void* buffer, size_t len, void* user_data);

typedef bool (*gsswrap_recv_token_fn)(
    void** buffer, size_t* len, void* user_data);

typedef void (*gsswrap_free_buffer_fn)(
    void* buffer, size_t len, void* user_data);

#endif
