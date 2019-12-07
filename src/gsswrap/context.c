#include "context.h"

#include <stdlib.h>

struct gsswrap_context* make_context(gsswrap_send_token_fn send_function,
                                     gsswrap_recv_token_fn recv_function,
                                     gsswrap_free_buffer_fn free_function)
{
    struct gsswrap_context* ctx = malloc(sizeof(struct gsswrap_context));
    ctx->send_fn = send_function;
    ctx->recv_fn = recv_function;
    ctx->free_fn = free_function;
    ctx->client_name = GSS_C_NO_NAME;
    init_status(&ctx->status);
    return ctx;
}

void destroy_context(struct gsswrap_context* ctx)
{
    gss_release_name(&ctx->status.minor, &ctx->client_name);
    deinit_status(&ctx->status);
    free(ctx);
}
