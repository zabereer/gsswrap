#include "client.h"

#include "context.h"

bool gsswrap_set_server_name(struct gsswrap_context* ctx,
                             const char* const principal,
                             const bool host_based)
{
    gsswrap_import_name(ctx, &ctx->server_name, principal, host_based);
    return !GSS_ERROR(ctx->major);
}
