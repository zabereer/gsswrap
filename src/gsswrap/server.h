#ifndef GSSWRAP_SERVER_H
#define GSSWRAP_SERVER_H

#include "common.h"

#include <stdbool.h>

/**
 * Set keytab filename. This overrides environment variable KRB5_KTNAME.
 */
void gsswrap_set_keytab_file(const char* const);

/**
 * Set server credential to principal. If principal is "server" then
 * kerberos will suffix it with the realm name (i.e. "server@REALM.COM").
 * If host_based flag is true then "server" will become
 * "server/host.com@REAL.COM" (host based service like ssh) where
 * "host.com" is the fully qualified host name.
 */
bool gsswrap_set_server_cred(struct gsswrap_context*,
                             const char* const principal,
                             const bool host_based);

#endif
