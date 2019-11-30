#include "client.h"

#include "context.h"

bool gsswrap_set_server_name(struct gsswrap_context* ctx,
                             const char* const principal,
                             const bool host_based)
{
    gsswrap_import_name(ctx, &ctx->server_name, principal, host_based);
    return !GSS_ERROR(ctx->major);
}

bool gsswrap_set_client_cred(struct gsswrap_context* ctx,
                             const char* const principal)
{
    gss_name_t client = GSS_C_NO_NAME;
    gsswrap_import_name(ctx, &client, principal, false);
    if (!GSS_ERROR(ctx->major))
        gsswrap_acquire_cred(ctx, &ctx->client_cred, client, GSS_C_INITIATE);
    return !GSS_ERROR(ctx->major);
}
