#include "common.h"

#include "context.h"
#include "credential.h"

struct gsswrap_credential* gsswrap_make_credential()
{
    return make_credential();
}

void gsswrap_destroy_credential(struct gsswrap_credential* gcred)
{
    destroy_credential(gcred);
}

struct gsswrap_context* gsswrap_make_context(
    gsswrap_send_token_fn send_function,
    gsswrap_recv_token_fn recv_function,
    gsswrap_free_buffer_fn free_function)
{
    return make_context(send_function, recv_function, free_function);
}

void gsswrap_destroy_context(struct gsswrap_context* gctx)
{
    destroy_context(gctx);
}

const char* gsswrap_last_credential_error(struct gsswrap_credential* gcred)
{
    return last_error(&gcred->status);
}

const char* gsswrap_last_context_error(struct gsswrap_context* gctx)
{
    return last_error(&gctx->status);
}
