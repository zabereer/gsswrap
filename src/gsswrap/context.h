#ifndef GSSWRAP_CONTEXT_H
#define GSSWRAP_CONTEXT_H

#include <gssapi/gssapi.h>

struct gsswrap_context
{
    OM_uint32 major;
    OM_uint32 minor;
    char* last_error;
};

#endif
