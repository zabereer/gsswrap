#include "context.h"

#include <stdlib.h>

struct gsswrap_context* make_context(gsswrap_send_token_fn send_function,
                                     gsswrap_recv_token_fn recv_function)
{
    struct gsswrap_context* ctx = malloc(sizeof(struct gsswrap_context));
    ctx->send_fn = send_function;
    ctx->recv_fn = recv_function;
    init_status(&ctx->status);
    return ctx;
}

void destroy_context(struct gsswrap_context* ctx)
{
    deinit_status(&ctx->status);
    free(ctx);
}
