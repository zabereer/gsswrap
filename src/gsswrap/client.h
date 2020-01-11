#ifndef GSSWRAP_CLIENT_H
#define GSSWRAP_CLIENT_H

#include "common.h"

#include <stdbool.h>

/**
 * Set the target server name with which this client wants to authenticate.
 * This function is the client side equivalent of gsswrap_set_server_cred().
 * Returns true if successful.
 * (Normally this function does not fail for an invalid server_name.)
 */
bool gsswrap_set_server_name(struct gsswrap_credential*,
                             const char* const server_name,
                             const bool host_based);

/**
 * Call one of these functions to setup the client identity in the credential.
 * The default credential will be obtained from the cache.
 * Returns true if successful.
 */
void gsswrap_set_client_cred_default(struct gsswrap_credential*);
bool gsswrap_set_client_cred(struct gsswrap_credential*,
                             const char* const principal);
bool gsswrap_set_client_cred_pw(struct gsswrap_credential*,
                                const char* const principal,
                                const char* const password);

/**
 * If a server needs to delegate a client's request to another server then
 * the server can setup the credential from the authenticated client.
 * Returns true if successful.
 */
bool gsswrap_set_client_cred_delegated(struct gsswrap_credential*,
                                       struct gsswrap_context*);

/**
 * Initiate a security token exchange to establish a security context
 * with the server. Before calling this a connection must be established to
 * the server.
 * The send/recv/free functions in gsswrap_context will be called one or
 * more times before gsswrap_initiate() returns.
 * Returns true if successful.
 */
bool gsswrap_initiate(const struct gsswrap_credential*,
                      struct gsswrap_context*,
                      void* user_data);

#endif
