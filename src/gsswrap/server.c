#include "server.h"

#include "context.h"
#include "credential.h"

#include <gssapi/gssapi.h>
#include <gssapi/gssapi_krb5.h>

#include <string.h>

#pragma GCC visibility push(default)

void gsswrap_set_keytab_file(const char* const keytabfile)
{
    krb5_gss_register_acceptor_identity(keytabfile);
}

bool gsswrap_set_server_cred(struct gsswrap_credential* gcred,
                             const char* const principal,
                             const bool host_based)
{
    import_name(gcred, &gcred->server_name, principal, host_based);
    if (!GSS_ERROR(gcred->status.major))
        acquire_cred(gcred,
                     &gcred->server_cred,
                     gcred->server_name,
                     GSS_C_ACCEPT);
    return !GSS_ERROR(gcred->status.major);
}

bool gsswrap_accept(const struct gsswrap_credential* gcred,
                    struct gsswrap_context* gctx,
                    void* user_data)
{
    reset_context(gctx);
    gss_buffer_desc input_token = GSS_C_EMPTY_BUFFER;
    gss_buffer_desc output_token = GSS_C_EMPTY_BUFFER;
    OM_uint32 minor;  // temporary minor error for cleanup functions
    bool input_token_read = false;
    bool established = false;

    gctx->status.major = GSS_S_CONTINUE_NEEDED;
    while (!established)
    {
        if (gctx->status.major & GSS_S_CONTINUE_NEEDED)
        {
            if (!gctx->recv_fn(&input_token.value,
                               &input_token.length,
                               user_data))
            {
                break;
            }
            input_token_read = true;
        }
        else if (gctx->status.major == GSS_S_COMPLETE)
        {
            established = true;
            break;
        }
        else
        {
            break; // no error nor continue, forbidden by rfc2743
        }

        gctx->status.major = gss_accept_sec_context(
            &gctx->status.minor,
            &gctx->gss_ctx,
            gcred->server_cred,
            &input_token,
            NULL,  // channel bindings
            &gctx->client_name,
            NULL,  // actual mechanism type
            &output_token,
            &gctx->ret_flags,
            NULL,  // actual context validity time
            &gctx->delegated_client_cred);

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
                break;
            }
        }

        // output_token was allocated by GSSAPI, free it using GSSAPI
        gss_release_buffer(&minor, &output_token);

        if (GSS_ERROR(gctx->status.major))
        {
            break;
        }
    }

    gss_release_buffer(&minor, &output_token);
    return established;
}

const char* gsswrap_client_principal(struct gsswrap_context* gctx)
{
    if (!gctx->client_display_name)
    {
        gss_buffer_desc buffer;
        gctx->status.major = gss_display_name(&gctx->status.minor,
                                              gctx->client_name,
                                              &buffer,
                                              NULL);
        if (!GSS_ERROR(gctx->status.major))
            gctx->client_display_name = strndup(buffer.value, buffer.length);
    }
    return gctx->client_display_name;
}

#pragma GCC visibility pop
