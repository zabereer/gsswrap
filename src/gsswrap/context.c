#include "context.h"

#include <stdlib.h>

struct gsswrap_context* make_context(gsswrap_send_token_fn send_function,
                                     gsswrap_recv_token_fn recv_function,
                                     gsswrap_free_buffer_fn free_function)
{
    struct gsswrap_context* gctx = malloc(sizeof(struct gsswrap_context));
    gctx->send_fn = send_function;
    gctx->recv_fn = recv_function;
    gctx->free_fn = free_function;
    gctx->client_name = GSS_C_NO_NAME;
    init_status(&gctx->status);
    return gctx;
}

void destroy_context(struct gsswrap_context* gctx)
{
    gss_release_name(&gctx->status.minor, &gctx->client_name);
    deinit_status(&gctx->status);
    free(gctx);
}
