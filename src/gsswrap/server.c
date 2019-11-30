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
    gsswrap_import_name(ctx, &ctx->server_name, principal, host_based);
    if (!GSS_ERROR(ctx->major))
        gsswrap_acquire_cred(ctx, &ctx->server_cred, ctx->server_name);
    return !GSS_ERROR(ctx->major);
}
