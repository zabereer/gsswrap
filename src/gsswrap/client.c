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

bool gsswrap_set_client_cred_pw(struct gsswrap_context* ctx,
                                const char* const principal,
                                const char* const password)
{
    gss_name_t client = GSS_C_NO_NAME;
    gsswrap_import_name(ctx, &client, principal, false);
    if (!GSS_ERROR(ctx->major))
        gsswrap_acquire_cred_pw(ctx,
                                &ctx->client_cred,
                                client,
                                GSS_C_INITIATE,
                                password);
    return !GSS_ERROR(ctx->major);
}

bool gsswrap_negotiate(struct gsswrap_context* ctx,
                       void* user_data)
{
    gss_ctx_id_t gss_ctx = GSS_C_NO_CONTEXT;
    gss_buffer_desc input_token = GSS_C_EMPTY_BUFFER;
    gss_buffer_desc output_token = GSS_C_EMPTY_BUFFER;
    OM_uint32 ret_flags;
    const OM_uint32 req_flags =
        GSS_C_MUTUAL_FLAG | GSS_C_CONF_FLAG | GSS_C_INTEG_FLAG |
        GSS_C_DELEG_FLAG | GSS_C_REPLAY_FLAG | GSS_C_SEQUENCE_FLAG;
    ctx->major = gss_init_sec_context(&ctx->minor,
                                     ctx->client_cred,
                                     &gss_ctx,
                                     ctx->server_name,
                                     GSS_C_NO_OID,
                                     req_flags,
                                     GSS_C_INDEFINITE,
                                     NULL, // channel bindings
                                     &input_token,
                                     NULL, // actual mechanism type
                                     &output_token,
                                     &ret_flags,
                                     NULL);

    (void)user_data;
    return true;
}
