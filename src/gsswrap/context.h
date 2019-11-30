#ifndef GSSWRAP_CONTEXT_H
#define GSSWRAP_CONTEXT_H

#include "callbacks.h"

#include <gssapi/gssapi.h>

#include <stdbool.h>

/**
 * Context used by both client and server side.
 */
struct gsswrap_context
{
    gsswrap_send_token_fn send_fn;
    gsswrap_recv_token_fn recv_fn;
    void* user_data;

    OM_uint32 major;
    OM_uint32 minor;
    char* last_error;

    gss_name_t server_name;
    gss_cred_id_t server_cred;
};

// Internal functions used by both client and server.
void gsswrap_import_name(struct gsswrap_context*,
                        gss_name_t*,
                        const char* const,
                        const bool);
void gsswrap_acquire_cred(struct gsswrap_context*,
                          gss_cred_id_t*,
                          const gss_name_t);

#endif
