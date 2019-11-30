#include "context.h"

#include <gssapi/gssapi_ext.h>

#include <string.h>

void gsswrap_import_name(struct gsswrap_context* ctx,
                        gss_name_t* name,
                        const char* const principal,
                        const bool host_based)
{
    gss_release_name(&ctx->minor, name);
    gss_buffer_desc buffer = {.length = strlen(principal),
                              .value = (void*)principal};

    ctx->major = gss_import_name(&ctx->minor,
                                 &buffer,
                                 host_based ? GSS_C_NT_HOSTBASED_SERVICE
                                            : GSS_C_NT_USER_NAME,
                                 name);
}

void gsswrap_acquire_cred(struct gsswrap_context* ctx,
                          gss_cred_id_t* cred,
                          const gss_name_t imported_name,
                          const gss_cred_usage_t usage)
{
    gss_release_cred(&ctx->minor, cred);
    ctx->major = gss_acquire_cred(&ctx->minor,
                                  imported_name,
                                  GSS_C_INDEFINITE,
                                  GSS_C_NO_OID_SET,
                                  usage,
                                  cred,
                                  NULL,  // no actual mechanism required
                                  NULL); // no actual validity time
}

void gsswrap_acquire_cred_pw(struct gsswrap_context* ctx,
                            gss_cred_id_t* cred,
                            const gss_name_t imported_name,
                            const gss_cred_usage_t usage,
                            const char* const password)
{
    gss_release_cred(&ctx->minor, cred);
    gss_buffer_desc pw_buffer = {.length = strlen(password),
                                 .value = (void*)password};
    ctx->major = gss_acquire_cred_with_password(&ctx->minor,
                                                imported_name,
                                                &pw_buffer,
                                                GSS_C_INDEFINITE,
                                                GSS_C_NO_OID_SET,
                                                usage,
                                                cred,
                                                NULL,
                                                NULL);
}