#ifndef GSSWRAP_CLIENT_H
#define GSSWRAP_CLIENT_H

#include "common.h"

#include <stdbool.h>

/**
 * 
 */
bool gsswrap_set_server_name(struct gsswrap_context*,
                             const char* const server_name,
                             const bool host_based);

/**
 * 
 */
bool gsswrap_set_client_cred(struct gsswrap_context*,
                             const char* const principal);

#endif
