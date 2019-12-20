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
 */
bool gsswrap_set_server_cred(struct gsswrap_credential*,
                             const char* const principal,
                             const bool host_based);

/**
 * 
 */
bool gsswrap_accept(const struct gsswrap_credential*,
                    struct gsswrap_context*,
                    void* user_data);

/**
 * 
 */
const char* gsswrap_client_principal(struct gsswrap_context*);

#endif
