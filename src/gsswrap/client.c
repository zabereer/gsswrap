#include "client.h"

#include "context.h"
#include "credential.h"

bool gsswrap_set_server_name(struct gsswrap_credential* gcred,
                             const char* const principal,
                             const bool host_based)
{
    import_name(gcred, &gcred->server_name, principal, host_based);
    return !GSS_ERROR(gcred->status.major);
}

void gsswrap_set_client_cred_default(struct gsswrap_credential* gcred)
{
    release_name(gcred, &gcred->client_name);
    release_cred(gcred, &gcred->client_cred);
}

bool gsswrap_set_client_cred(struct gsswrap_credential* gcred,
                             const char* const principal)
{
    import_name(gcred, &gcred->client_name, principal, false);
    if (!GSS_ERROR(gcred->status.major))
        acquire_cred(gcred,
                     &gcred->client_cred,
                     gcred->client_name,
                     GSS_C_INITIATE);
    return !GSS_ERROR(gcred->status.major);
}

bool gsswrap_set_client_cred_pw(struct gsswrap_credential* gcred,
                                const char* const principal,
                                const char* const password)
{
    import_name(gcred, &gcred->client_name, principal, false);
    if (!GSS_ERROR(gcred->status.major))
        acquire_cred_pw(gcred,
                        &gcred->client_cred,
                        gcred->client_name,
                        GSS_C_INITIATE,
                        password);
    return !GSS_ERROR(gcred->status.major);
}

bool gsswrap_set_client_cred_delegated(struct gsswrap_credential* gcred,
                                       struct gsswrap_context* gctx)
{
    if (gsswrap_delegated(gctx) &&
        gctx->delegated_client_cred != GSS_C_NO_CREDENTIAL)
    {
        release_name(gcred, &gcred->client_name);
        release_cred(gcred, &gcred->client_cred);
        gcred->client_name = gctx->client_name;
        gcred->client_cred = gctx->delegated_client_cred;
        gctx->client_name = GSS_C_NO_NAME;
        gctx->delegated_client_cred = GSS_C_NO_CREDENTIAL;
        return true;
    }
    return false;
}

bool gsswrap_initiate(const struct gsswrap_credential* gcred,
                      struct gsswrap_context* gctx,
                      void* user_data)
{
    reset_context(gctx);
    gss_ctx_id_t gss_ctx = GSS_C_NO_CONTEXT;
    gss_buffer_desc input_token = GSS_C_EMPTY_BUFFER;
    gss_buffer_desc output_token = GSS_C_EMPTY_BUFFER;
    if (!gctx->req_flags)
        gsswrap_set_all_flags(gctx);

    OM_uint32 minor;  // temporary minor error for cleanup functions
    bool input_token_read = false;
    bool established = false;

    while (!established)
    {
        gctx->status.major = gss_init_sec_context(
            &gctx->status.minor,
            gcred->client_cred,
            &gss_ctx,
            gcred->server_name,
            GSS_C_NO_OID,
            gctx->req_flags,
            GSS_C_INDEFINITE, // maximum permitted lifetime
            NULL,  // channel bindings
            &input_token,
            NULL,  // actual mechanism type
            &output_token,
            &gctx->ret_flags,
            NULL); // actual context validity time

        // free input_token immediately as it was just consumed
        if (input_token_read)
        {
            gctx->free_fn(input_token.value, input_token.length, user_data);
            input_token_read = false;
        }

        if ((gctx->status.major & GSS_S_CONTINUE_NEEDED) ||
            output_token.length)
        {
            // always send token if requested (GSS_S_CONTINUE_NEEDED)
            // or any is present
            if (!gctx->send_fn(output_token.value,
                               output_token.length,
                               user_data))
            {
                goto cleanup;
            }
        }

        // output_token was allocated by GSSAPI, free it using GSSAPI
        gss_release_buffer(&minor, &output_token);

        if (gctx->status.major & GSS_S_CONTINUE_NEEDED)
        {
            if (!gctx->recv_fn(&input_token.value,
                               &input_token.length,
                               user_data))
            {
                goto cleanup;
            }
            input_token_read = true;
        }
        else if (gctx->status.major == GSS_S_COMPLETE)
        {
            established = true;
        }

        if (GSS_ERROR(gctx->status.major))
        {
            break;
        }
    }

cleanup:
    gss_release_buffer(&minor, &output_token);
    // TODO maybe keep context somewhere for encrypted exchange or delegation?
    gss_delete_sec_context(&gctx->status.minor, &gss_ctx, GSS_C_NO_BUFFER);
    return established;
}
