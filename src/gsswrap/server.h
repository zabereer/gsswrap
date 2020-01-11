#ifndef GSSWRAP_SERVER_H
#define GSSWRAP_SERVER_H

#include "common.h"

#include <stdbool.h>

/**
 * Set keytab filename. This overrides environment variable KRB5_KTNAME
 * and is a global setting for Kerberos.
 */
void gsswrap_set_keytab_file(const char* const);

/**
 * Set server credential to principal. If principal is "server" then
 * kerberos will suffix it with the realm name (i.e. "server@REALM.COM").
 * If host_based flag is true then "server" will become
 * "server/host.com@REAL.COM" (host based service like ssh) where
 * "host.com" is the fully qualified host name.
 * Returns true if successful.
 */
bool gsswrap_set_server_cred(struct gsswrap_credential*,
                             const char* const principal,
                             const bool host_based);

/**
 * Accept a security token exchange request from a client. Before calling
 * this function a client connection must have been accepted.
 * The send/recv/free functions in gsswrap_context will be called one or
 * more times before gsswrap_accept() returns.
 * Returns true if successful.
 */
bool gsswrap_accept(const struct gsswrap_credential*,
                    struct gsswrap_context*,
                    void* user_data);

/**
 * If gsswrap_accept() was successful then the authenticated client
 * principal can be retrieved using this function.
 * Returns NULL if not successful (gsswrap_accept() did not succeed).
 */
const char* gsswrap_client_principal(struct gsswrap_context*);

#endif
