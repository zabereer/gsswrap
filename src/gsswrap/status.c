#include "status.h"

#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void init_status(struct gsswrap_status* gs)
{
    gs->major = GSS_S_COMPLETE;
    gs->minor = GSS_S_COMPLETE;
    gs->last_error = NULL;
}

void deinit_status(struct gsswrap_status* gs)
{
    free((void*)gs->last_error);
}

static void append_string_to_last_string(struct gsswrap_status* gs,
                                         const char* string)
{
    if (gs->last_error)
    {
        gs->last_error = realloc(
            gs->last_error,
            strlen(gs->last_error) + strlen(string) + 1);
        strcat(gs->last_error, string);
    }
    else
    {
        gs->last_error = malloc(strlen(string) + 1);
        strcpy(gs->last_error, string);
    }
}

static void append_buffer_to_last_error(struct gsswrap_status* gs,
                                        const gss_buffer_t status_string)
{
    char buffer[status_string->length + 10];
    int len =
        status_string->length < INT_MAX ? status_string->length : INT_MAX;
    snprintf(buffer, sizeof(buffer), "[ %.*s ]",
             len, (char*)status_string->value);
    append_string_to_last_string(gs, buffer);
}

static void append_codes_to_last_error(struct gsswrap_status* gs,
                                       const OM_uint32 major,
                                       const OM_uint32 minor)
{
    char buffer[64];
    snprintf(buffer, sizeof(buffer), "[ major=%u minor=%u ]", major, minor);
    append_string_to_last_string(gs, buffer);
}

static void set_last_error(struct gsswrap_status* gs)
{
    OM_uint32 message_context = 0;
    // do not call gss_display_status for unknown GSS failure
    int status_type =
        gs->major == GSS_S_FAILURE ? GSS_C_MECH_CODE : GSS_C_GSS_CODE;
    while (true)
    {
        OM_uint32 minor;
        OM_uint32 code =
            status_type == GSS_C_GSS_CODE ? gs->major : gs->minor;
        gss_buffer_desc status_string = GSS_C_EMPTY_BUFFER;
        OM_uint32 major = gss_display_status(&minor,
                                             code,
                                             status_type,
                                             GSS_C_NO_OID,
                                             &message_context,
                                             &status_string);
        if (major == GSS_S_COMPLETE)
            append_buffer_to_last_error(gs, &status_string);
        else
            append_codes_to_last_error(gs, gs->major, gs->minor);

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

const char* last_error(struct gsswrap_status* gs)
{
    free((void*)gs->last_error);
    gs->last_error = NULL;
    if (gs->major != GSS_S_COMPLETE)
        set_last_error(gs);
    return gs->last_error;
}
