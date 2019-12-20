#ifndef GSSWRAP_CONTEXT_H
#define GSSWRAP_CONTEXT_H

#include "callbacks.h"
#include "status.h"

#include <gssapi/gssapi.h>

/**
 * Context used by both client and server side.
 */
struct gsswrap_context
{
    gsswrap_send_token_fn send_fn;
    gsswrap_recv_token_fn recv_fn;
    gsswrap_free_buffer_fn free_fn;

    // populated by server acceptor
    gss_name_t client_name;
    const char* client_display_name;

    struct gsswrap_status status;
};

// Internal functions used by both client and server.
struct gsswrap_context* make_context(gsswrap_send_token_fn,
                                     gsswrap_recv_token_fn,
                                     gsswrap_free_buffer_fn);
void reset_context(struct gsswrap_context*);
void destroy_context(struct gsswrap_context*);

#endif
