# gsswrap
This library wraps the GSS-API ([RFC2743](https://tools.ietf.org/html/rfc2743.html)) negotiation loop ([RFC7546](https://tools.ietf.org/html/rfc7546.html)) for ease of use by most applications.
It is published under [MIT license](LICENSE).

## Example usage
gsswrap can be used to write server and client code to authenticate client connections using kerberos. All interface functions and types are prefixed with "gsswrap_".

### Server example
Error checking omitted for brevity.
```C
#include "gsswrap/server.h"

// establish server credential
struct gsswrap_credential* cred = gsswrap_make_credential();
gsswrap_set_server_cred(cred, "server-principal-name", true);

// after establishing a listener socket do this for each client connection

while (serving_clients) {
    // create a context for this client
    struct gsswrap_context* ctx = gsswrap_make_context(send_function,
                                                       recv_function,
                                                       free_function);

    // after accepting client connection then run negotiation loop
    // gsswrap_accept() will call functions pointeed to by ctx
    if (gsswrap_accept(cred, ctx, user_data))
        verify_client_principal(gsswrap_client_principal(ctx));

    gsswrap_destroy_context(ctx;
}

gsswrap_destroy_credential(cred);
```

### Client example
Error checking omitted for brevity.
```C
#include "gsswrap/client.h"

// establish client credential using username and password
struct gsswrap_credential* cred = gsswrap_make_credential();
gsswrap_set_client_cred_pw(cred, "batman", "forever");

// set the server name we want to authenticate with
gsswrap_set_server_name(cred, "server-principal-name", true);

// after connecting to server

struct gsswrap_context* ctx = gsswrap_make_context(send_function,
                                                   recv_function,
                                                   free_function);
if (gsswrap_initiate(cred, ctx, user_data))
    handle_authenticated_connection();

gsswrap_destroy_context(ctx;
gsswrap_destroy_credential(cred);
```

### Credential delegation


## Build from checked out repo
gsswrap uses standard autoconf/automake.
From the checked out gsswrap directory:
```
autoreconf -ivf
mkdir build
cd build
../configure  # use --help for options
make
```
To run tests:
```
make check
```
To redistribute source:
```
make dist
```
