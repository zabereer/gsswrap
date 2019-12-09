#ifndef GSSWRAP_CLIENT_H
#define GSSWRAP_CLIENT_H

#include "common.h"

#include <stdbool.h>

/**
 * (Normally this does not fail for an invalid server_name.)
 */
bool gsswrap_set_server_name(struct gsswrap_credential*,
                             const char* const server_name,
                             const bool host_based);

/**
 * 
 */
bool gsswrap_set_client_cred(struct gsswrap_credential*,
                             const char* const principal);
bool gsswrap_set_client_cred_pw(struct gsswrap_credential*,
                                const char* const principal,
                                const char* const password);

/**
 * 
 */
bool gsswrap_initiate(const struct gsswrap_credential*,
                      struct gsswrap_context*,
                      void* user_data);
#endif
