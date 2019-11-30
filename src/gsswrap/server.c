#include "server.h"

#include "context.h"

//#include <gssapi/gssapi.h>
//#include <gssapi/gssapi_ext.h>
#include <gssapi/gssapi_krb5.h>

#include <string.h>

void gsswrap_set_keytab_file(const char* const keytabfile)
{
    krb5_gss_register_acceptor_identity(keytabfile);
}

bool gsswrap_set_server_cred(struct gsswrap_context* ctx,
                             const char* const principal,
                             const bool host_based)
{
    gss_release_name(&ctx->minor, &ctx->server_name);
    gss_release_cred(&ctx->minor, &ctx->server_cred);
    gss_buffer_desc buffer = {.length = strlen(principal),
                              .value = (void*)principal};

    ctx->major = gss_import_name(&ctx->minor,
                                 &buffer,
                                 host_based ? GSS_C_NT_HOSTBASED_SERVICE
                                            : GSS_C_NT_USER_NAME,
                                 &ctx->server_name);
    if (!GSS_ERROR(ctx->major))
    {
        ctx->major = gss_acquire_cred(&ctx->minor,
                                      ctx->server_name,
                                      GSS_C_INDEFINITE,
                                      GSS_C_NO_OID_SET,
                                      GSS_C_ACCEPT,
                                      &ctx->server_cred,
                                      NULL,  // no actual mechanism required
                                      NULL); // no actual validity time
    }
    return !GSS_ERROR(ctx->major);
}
