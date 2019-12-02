#include "client.h"

#include "context.h"
#include "credential.h"

bool gsswrap_set_server_name(struct gsswrap_credential* gc,
                             const char* const principal,
                             const bool host_based)
{
    import_name(gc, &gc->server_name, principal, host_based);
    return !GSS_ERROR(gc->status.major);
}

bool gsswrap_set_client_cred(struct gsswrap_credential* gc,
                             const char* const principal)
{
    gss_name_t client = GSS_C_NO_NAME;
    import_name(gc, &client, principal, false);
    if (!GSS_ERROR(gc->status.major))
        acquire_cred(gc, &gc->client_cred, client, GSS_C_INITIATE);
    return !GSS_ERROR(gc->status.major);
}

bool gsswrap_set_client_cred_pw(struct gsswrap_credential* gc,
                                const char* const principal,
                                const char* const password)
{
    gss_name_t client = GSS_C_NO_NAME;
    import_name(gc, &client, principal, false);
    if (!GSS_ERROR(gc->status.major))
        acquire_cred_pw(gc,
                        &gc->client_cred,
                        client,
                        GSS_C_INITIATE,
                        password);
    return !GSS_ERROR(gc->status.major);
}

bool gsswrap_negotiate(const struct gsswrap_credential* gc,
                       struct gsswrap_context* ctx,
                       void* user_data)
{
    gss_ctx_id_t gss_ctx = GSS_C_NO_CONTEXT;
    gss_buffer_desc input_token = GSS_C_EMPTY_BUFFER;
    gss_buffer_desc output_token = GSS_C_EMPTY_BUFFER;
    OM_uint32 ret_flags;
    const OM_uint32 req_flags =
        GSS_C_MUTUAL_FLAG | GSS_C_CONF_FLAG | GSS_C_INTEG_FLAG |
        GSS_C_DELEG_FLAG | GSS_C_REPLAY_FLAG | GSS_C_SEQUENCE_FLAG;

    OM_uint32 minor;  // temporary minor error for cleanup functions
    bool established = false;

    while (!established)
    {
        ctx->status.major = gss_init_sec_context(
            &ctx->status.minor,
            gc->client_cred,
            &gss_ctx,
            gc->server_name,
            GSS_C_NO_OID,
            req_flags,
            GSS_C_INDEFINITE, // maximum permitted lifetime
            NULL,  // channel bindings
            &input_token,
            NULL,  // actual mechanism type
            &output_token,
            &ret_flags,
            NULL); // actual context validity time

        // free input_token immediately as it was just consumed
        // TODO how to free input_token

        if ((ctx->status.major & GSS_S_CONTINUE_NEEDED) ||
            output_token.length)
        {
            // always send token if requested (GSS_S_CONTINUE_NEEDED)
            // or any is present
            if (!ctx->send_fn(output_token.value,
                              output_token.length,
                              user_data))
            {
                goto cleanup;
            }
        }

        // output_token was allocated by GSSAPI, free it using GSSAPI
        gss_release_buffer(&minor, &output_token);

        if (ctx->status.major & GSS_S_CONTINUE_NEEDED)
        {
            // TODO receive new input_token from peer
        }
        else if (ctx->status.major & GSS_S_COMPLETE)
        {
            established = true;
        }
        else if (GSS_ERROR(ctx->status.major))
        {
            break;
        }
    }

    // TODO verify requested flags were used by checking
    // if ((ret_flags & req_flags) != req_flags) ...

cleanup:
    gss_release_buffer(&minor, &output_token);
    // TODO maybe keep context somewhere for encrypted exchange or delegation?
    gss_delete_sec_context(&ctx->status.minor, &gss_ctx, GSS_C_NO_BUFFER);
    return established;
}
