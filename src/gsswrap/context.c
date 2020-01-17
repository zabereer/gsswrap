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
    gctx->gss_ctx = GSS_C_NO_CONTEXT;
    gctx->client_name = GSS_C_NO_NAME;
    gctx->client_display_name = NULL;
    gctx->delegated_client_cred = GSS_C_NO_CREDENTIAL;
    init_status(&gctx->status);
    gctx->req_flags = 0;
    gctx->ret_flags = 0;
    gss_buffer_desc empty = GSS_C_EMPTY_BUFFER;
    gctx->wrap_buffer = empty;
    return gctx;
}

void reset_context(struct gsswrap_context* gctx)
{
    gss_delete_sec_context(&gctx->status.minor, &gctx->gss_ctx,
                           GSS_C_NO_BUFFER);
    gctx->gss_ctx = GSS_C_NO_CONTEXT;
    gss_release_name(&gctx->status.minor, &gctx->client_name);
    gctx->client_name = GSS_C_NO_NAME;
    free((void*)gctx->client_display_name);
    gctx->client_display_name = NULL;
    gss_release_cred(&gctx->status.minor, &gctx->delegated_client_cred);
    gctx->delegated_client_cred = GSS_C_NO_CREDENTIAL;
    gss_release_buffer(&gctx->status.minor, &gctx->wrap_buffer);
}

void destroy_context(struct gsswrap_context* gctx)
{
    reset_context(gctx);
    deinit_status(&gctx->status);
    free(gctx);
}

void reset_wrap_buffer(struct gsswrap_context* gctx)
{
    gss_release_buffer(&gctx->status.minor, &gctx->wrap_buffer);
    gss_buffer_desc empty = GSS_C_EMPTY_BUFFER;
    gctx->wrap_buffer = empty;
}
