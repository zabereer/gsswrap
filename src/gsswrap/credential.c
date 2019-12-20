#include "credential.h"

#include <gssapi/gssapi_ext.h>

#include <stdlib.h>
#include <string.h>

struct gsswrap_credential* make_credential()
{
    struct gsswrap_credential* gcred =
        malloc(sizeof(struct gsswrap_credential));
    gcred->server_name = GSS_C_NO_NAME;
    gcred->server_cred = GSS_C_NO_CREDENTIAL;
    gcred->client_name = GSS_C_NO_NAME;
    gcred->client_cred = GSS_C_NO_CREDENTIAL;
    init_status(&gcred->status);
    return gcred;
}

void destroy_credential(struct gsswrap_credential* gcred)
{
    gss_release_name(&gcred->status.minor, &gcred->server_name);
    gss_release_cred(&gcred->status.minor, &gcred->server_cred);
    gss_release_name(&gcred->status.minor, &gcred->client_name);
    gss_release_cred(&gcred->status.minor, &gcred->client_cred);
    deinit_status(&gcred->status);
    free(gcred);
}

void import_name(struct gsswrap_credential* gcred,
                 gss_name_t* name,
                 const char* const principal,
                 const bool host_based)
{
    gss_release_name(&gcred->status.minor, name);
    gss_buffer_desc buffer = {.length = strlen(principal),
                              .value = (void*)principal};

    gcred->status.major = gss_import_name(&gcred->status.minor,
                                       &buffer,
                                       host_based ? GSS_C_NT_HOSTBASED_SERVICE
                                                  : GSS_C_NT_USER_NAME,
                                       name);
}

void acquire_cred(struct gsswrap_credential* gcred,
                  gss_cred_id_t* cred,
                  const gss_name_t imported_name,
                  const gss_cred_usage_t usage)
{
    gss_release_cred(&gcred->status.minor, cred);
    gcred->status.major = gss_acquire_cred(&gcred->status.minor,
                                        imported_name,
                                        GSS_C_INDEFINITE,
                                        GSS_C_NO_OID_SET,
                                        usage,
                                        cred,
                                        NULL,  // no actual mechanism required
                                        NULL); // no actual validity time
}

void acquire_cred_pw(struct gsswrap_credential* gcred,
                     gss_cred_id_t* cred,
                     const gss_name_t imported_name,
                     const gss_cred_usage_t usage,
                     const char* const password)
{
    gss_release_cred(&gcred->status.minor, cred);
    gss_buffer_desc pw_buffer = {.length = strlen(password),
                                 .value = (void*)password};
    gcred->status.major = gss_acquire_cred_with_password(&gcred->status.minor,
                                                      imported_name,
                                                      &pw_buffer,
                                                      GSS_C_INDEFINITE,
                                                      GSS_C_NO_OID_SET,
                                                      usage,
                                                      cred,
                                                      NULL,
                                                      NULL);
}
