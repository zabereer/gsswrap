#include "common.h"

#include "context.h"

struct gsswrap_context* gsswrap_make_context(
    gsswrap_send_token_fn send_function,
    gsswrap_recv_token_fn recv_function,
    gsswrap_free_buffer_fn free_function)
{
    return make_context(send_function, recv_function, free_function);
}

void gsswrap_destroy_context(struct gsswrap_context* ctx)
{
    destroy_context(ctx);
}

const char* gsswrap_last_error(struct gsswrap_context* ctx)
{
    return last_error(&ctx->status);
}
