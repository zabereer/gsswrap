#ifndef GSSWRAP_CREDENTIAL_H
#define GSSWRAP_CREDENTIAL_H

#include "status.h"

#include <gssapi/gssapi.h>

#include <stdbool.h>

/**
 * Context used by both client and server side.
 */
struct gsswrap_credential
{
    gss_name_t server_name;
    gss_cred_id_t server_cred;

    gss_name_t client_name;
    gss_cred_id_t client_cred;

    struct gsswrap_status status;
};

// Internal functions used by both client and server.
struct gsswrap_credential* make_credential();
void destroy_credential(struct gsswrap_credential*);
void import_name(struct gsswrap_credential*,
                 gss_name_t*,
                 const char* const,
                 const bool);
void release_name(struct gsswrap_credential*,
                  gss_name_t*);
void acquire_cred(struct gsswrap_credential*,
                  gss_cred_id_t*,
                  const gss_name_t,
                  const gss_cred_usage_t);
void acquire_cred_pw(struct gsswrap_credential*,
                     gss_cred_id_t*,
                     const gss_name_t,
                     const gss_cred_usage_t,
                     const char* const);
void release_cred(struct gsswrap_credential*,
                  gss_cred_id_t*);
#endif
