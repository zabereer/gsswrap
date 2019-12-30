#include "common.h"

#include "context.h"
#include "credential.h"

#pragma GCC visibility push(default)

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

void gsswrap_set_confidentiality_available(struct gsswrap_context* gctx)
{
    gctx->req_flags |= GSS_C_CONF_FLAG;
}

void gsswrap_set_delegated(struct gsswrap_context* gctx)
{
    gctx->req_flags |= GSS_C_DELEG_FLAG;
}

void gsswrap_set_integrity_available(struct gsswrap_context* gctx)
{
    gctx->req_flags |= GSS_C_INTEG_FLAG;
}

void gsswrap_set_mutual_auth(struct gsswrap_context* gctx)
{
    gctx->req_flags |= GSS_C_MUTUAL_FLAG;
}

void gsswrap_set_out_of_sequence_detection(struct gsswrap_context* gctx)
{
    gctx->req_flags |= GSS_C_SEQUENCE_FLAG;
}

void gsswrap_set_replay_detection(struct gsswrap_context* gctx)
{
    gctx->req_flags |= GSS_C_REPLAY_FLAG;
}

void gsswrap_set_all_flags(struct gsswrap_context* gctx)
{
    gctx->req_flags = GSS_C_CONF_FLAG | GSS_C_DELEG_FLAG | GSS_C_INTEG_FLAG |
        GSS_C_MUTUAL_FLAG | GSS_C_SEQUENCE_FLAG | GSS_C_REPLAY_FLAG;
}

void gsswrap_clear_all_flags(struct gsswrap_context* gctx)
{
    gctx->req_flags = 0;
}

bool gsswrap_confidentiality_available(const struct gsswrap_context* gctx)
{
    return gctx->ret_flags & GSS_C_CONF_FLAG;
}

bool gsswrap_delegated(const struct gsswrap_context* gctx)
{
    return gctx->ret_flags & GSS_C_DELEG_FLAG;
}

bool gsswrap_integrity_available(const struct gsswrap_context* gctx)
{
    return gctx->ret_flags & GSS_C_INTEG_FLAG;
}

bool gsswrap_mutual_auth(const struct gsswrap_context* gctx)
{
    return gctx->ret_flags & GSS_C_MUTUAL_FLAG;
}

bool gsswrap_out_of_sequence_detection(const struct gsswrap_context* gctx)
{
    return gctx->ret_flags & GSS_C_SEQUENCE_FLAG;
}

bool gsswrap_replay_detection(const struct gsswrap_context* gctx)
{
    return gctx->ret_flags & GSS_C_REPLAY_FLAG;
}

#pragma GCC visibility pop
