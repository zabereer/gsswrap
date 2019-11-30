#ifndef GSSWRAP_SERVER_H
#define GSSWRAP_SERVER_H

#include "common.h"

#include <stdbool.h>

/**
 * Set keytab filename. This overrides environment variable KRB5_KTNAME.
 */
void gsswrap_set_keytab_file(const char* const);

/**
 * 
 */
bool gsswrap_set_server_cred(struct gsswrap_context*,
                             const char* const principal,
                             const bool host_based);

#endif
