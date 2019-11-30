#include "server.h"

//#include <gssapi/gssapi.h>
//#include <gssapi/gssapi_ext.h>
#include <gssapi/gssapi_krb5.h>

void gsswrap_set_keytab_file(const char* const keytabfile)
{
    krb5_gss_register_acceptor_identity(keytabfile);
}