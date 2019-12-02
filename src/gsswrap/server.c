#include "server.h"

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
