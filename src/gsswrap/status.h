#ifndef GSSWRAP_STATUS_H
#define GSSWRAP_STATUS_H

#include <gssapi/gssapi.h>

struct gsswrap_status
{
    OM_uint32 major;
    OM_uint32 minor;
    char* last_error;
};

// Internal functions used by both client and server.
void init_status(struct gsswrap_status*);
void deinit_status(struct gsswrap_status*);
const char* last_error(struct gsswrap_status*);

#endif
