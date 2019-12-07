#include "server.h"

#include "context.h"
#include "credential.h"

#include <gssapi/gssapi.h>
#include <gssapi/gssapi_krb5.h>

void gsswrap_set_keytab_file(const char* const keytabfile)
{
    krb5_gss_register_acceptor_identity(keytabfile);
}

bool gsswrap_set_server_cred(struct gsswrap_credential* gc,
                             const char* const principal,
                             const bool host_based)
{
    import_name(gc, &gc->server_name, principal, host_based);
    if (!GSS_ERROR(gc->status.major))
        acquire_cred(gc,
                     &gc->server_cred,
                     gc->server_name,
                     GSS_C_ACCEPT);
    return !GSS_ERROR(gc->status.major);
}

bool gsswrap_accept(const struct gsswrap_credential* gc,
                    struct gsswrap_context* ctx,
                    void* user_data)
{
    gss_ctx_id_t gss_ctx = GSS_C_NO_CONTEXT;
    gss_buffer_desc input_token = GSS_C_EMPTY_BUFFER;
    gss_buffer_desc output_token = GSS_C_EMPTY_BUFFER;
    OM_uint32 ret_flags;
    OM_uint32 minor;  // temporary minor error for cleanup functions
    bool input_token_read = false;
    bool established = false;

    ctx->status.major = GSS_S_CONTINUE_NEEDED;
    while (!established)
    {
        if (ctx->status.major & GSS_S_CONTINUE_NEEDED)
        {
            if (!ctx->recv_fn(&input_token.value,
                              &input_token.length,
                              user_data))
            {
                break;
            }
            input_token_read = true;
        }
        else if (ctx->status.major == GSS_S_COMPLETE)
        {
            established = true;
            break;
        }
        else
        {
            break; // no error nor continue, forbidden by rfc2743
        }

        ctx->status.major = gss_accept_sec_context(
            &ctx->status.minor,
            &gss_ctx,
            gc->server_cred,
            &input_token,
            NULL,  // channel bindings
            &ctx->client_name,
            NULL,  // actual mechanism type
            &output_token,
            &ret_flags,
            NULL,  // actual context validity time
            NULL); // delegated credentials - TODO handle this

        if (input_token_read)
        {
            ctx->free_fn(input_token.value, input_token.length, user_data);
            input_token_read = false;
        }

        if ((ctx->status.major & GSS_S_CONTINUE_NEEDED) ||
            output_token.length)
        {
            // always send token if requested (GSS_S_CONTINUE_NEEDED)
            // or any is present
            if (!ctx->send_fn(output_token.value,
                              output_token.length,
                              user_data))
            {
                break;
            }
        }

        // output_token was allocated by GSSAPI, free it using GSSAPI
        gss_release_buffer(&minor, &output_token);

        if (GSS_ERROR(ctx->status.major))
        {
            break;
        }
    }

    // TODO verify ret_flags are acceptable (integrity??)
    // if (!(ret_flags & GSS_C_INTEG_FLAG)) ... and then what if not?

// cleanup:
    gss_release_buffer(&minor, &output_token);
    // TODO maybe keep context somewhere for encrypted exchange or delegation?
    gss_delete_sec_context(&ctx->status.minor, &gss_ctx, GSS_C_NO_BUFFER);
    return established;
}