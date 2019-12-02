#include "credential.h"

#include <gssapi/gssapi_ext.h>

#include <stdlib.h>
#include <string.h>

struct gsswrap_credential* make_credential()
{
    struct gsswrap_credential* gc = malloc(sizeof(struct gsswrap_credential));
    gc->server_name = GSS_C_NO_NAME;
    gc->server_cred = GSS_C_NO_CREDENTIAL;
    gc->client_cred = GSS_C_NO_CREDENTIAL;
    init_status(&gc->status);
    return gc;
}

void destroy_credential(struct gsswrap_credential* gc)
{
    gss_release_name(&gc->status.minor, &gc->server_name);
    gss_release_cred(&gc->status.minor, &gc->server_cred);
    gss_release_cred(&gc->status.minor, &gc->client_cred);
    deinit_status(&gc->status);
    free(gc);
}

void import_name(struct gsswrap_credential* gc,
                 gss_name_t* name,
                 const char* const principal,
                 const bool host_based)
{
    gss_release_name(&gc->status.minor, name);
    gss_buffer_desc buffer = {.length = strlen(principal),
                              .value = (void*)principal};

    gc->status.major = gss_import_name(&gc->status.minor,
                                       &buffer,
                                       host_based ? GSS_C_NT_HOSTBASED_SERVICE
                                                  : GSS_C_NT_USER_NAME,
                                       name);
}

void acquire_cred(struct gsswrap_credential* gc,
                  gss_cred_id_t* cred,
                  const gss_name_t imported_name,
                  const gss_cred_usage_t usage)
{
    gss_release_cred(&gc->status.minor, cred);
    gc->status.major = gss_acquire_cred(&gc->status.minor,
                                        imported_name,
                                        GSS_C_INDEFINITE,
                                        GSS_C_NO_OID_SET,
                                        usage,
                                        cred,
                                        NULL,  // no actual mechanism required
                                        NULL); // no actual validity time
}

void acquire_cred_pw(struct gsswrap_credential* gc,
                     gss_cred_id_t* cred,
                     const gss_name_t imported_name,
                     const gss_cred_usage_t usage,
                     const char* const password)
{
    gss_release_cred(&gc->status.minor, cred);
    gss_buffer_desc pw_buffer = {.length = strlen(password),
                                 .value = (void*)password};
    gc->status.major = gss_acquire_cred_with_password(&gc->status.minor,
                                                      imported_name,
                                                      &pw_buffer,
                                                      GSS_C_INDEFINITE,
                                                      GSS_C_NO_OID_SET,
                                                      usage,
                                                      cred,
                                                      NULL,
                                                      NULL);
}
