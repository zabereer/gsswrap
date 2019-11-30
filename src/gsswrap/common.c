#include "common.h"

#include "context.h"

#include <gssapi/gssapi.h>

#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct gsswrap_context* gsswrap_make_context()
{
    struct gsswrap_context* ctx = malloc(sizeof(struct gsswrap_context));
    ctx->major = GSS_S_COMPLETE;
    ctx->minor = GSS_S_COMPLETE;
    ctx->last_error = NULL;
    return ctx;
}

void gsswrap_free_context(struct gsswrap_context* ctx)
{
    free((void*)ctx->last_error);
    free(ctx);
}

static void append_string_to_last_string(struct gsswrap_context* ctx,
                                         const char* string)
{
    if (ctx->last_error)
    {
        ctx->last_error = realloc(
            ctx->last_error,
            strlen(ctx->last_error) + strlen(string) + 1);
        strcat(ctx->last_error, string);
    }
    else
    {
        ctx->last_error = malloc(strlen(string) + 1);
        strcpy(ctx->last_error, string);
    }
}

static void append_buffer_to_last_error(struct gsswrap_context* ctx,
                                        const gss_buffer_t status_string)
{
    char buffer[status_string->length + 10];
    int len =
        status_string->length < INT_MAX ? status_string->length : INT_MAX;
    snprintf(buffer, sizeof(buffer), "[ %.*s ]", len, status_string->value);
    append_string_to_last_string(ctx, buffer);
}

static void append_codes_to_last_error(struct gsswrap_context* ctx,
                                       const OM_uint32 major,
                                       const OM_uint32 minor)
{
    char buffer[64];
    snprintf(buffer, sizeof(buffer), "[ major=%u minor=%u ]", major, minor);
    append_string_to_last_string(ctx, buffer);
}

static void set_last_error(struct gsswrap_context* ctx)
{
    OM_uint32 message_context = 0;
    gss_buffer_desc status_string;
    // do not call gss_display_status for unknown GSS failure
    int status_type =
        ctx->major == GSS_S_FAILURE ? GSS_C_MECH_CODE : GSS_C_GSS_CODE;
    while (true)
    {
        OM_uint32 minor;
        OM_uint32 code =
            status_type == GSS_C_GSS_CODE ? ctx->major : ctx->minor;
        gss_buffer_desc status_string = {.length = 0, .value = NULL};
        OM_uint32 major = gss_display_status(&minor,
                                             code,
                                             status_type,
                                             GSS_C_NO_OID,
                                             &message_context,
                                             &status_string);
        if (major == GSS_S_COMPLETE)
            append_buffer_to_last_error(ctx, &status_string);
        else
            append_codes_to_last_error(ctx, ctx->major, ctx->minor);

        gss_release_buffer(&minor, &status_string);
        if (!message_context)
        {
            if (status_type == GSS_C_GSS_CODE)
                status_type = GSS_C_MECH_CODE;
            else
                break;
        }
    }
}

const char* gsswrap_last_error(struct gsswrap_context* ctx)
{
    free((void*)ctx->last_error);
    ctx->last_error = NULL;
    if (ctx->major != GSS_S_COMPLETE)
        set_last_error(ctx);
    return ctx->last_error;
}
