# gsswrap
This library wraps the GSS-API ([RFC2743](https://tools.ietf.org/html/rfc2743.html)) negotiation loop ([RFC7546](https://tools.ietf.org/html/rfc7546.html)) for ease of use by most applications.
It is published under [MIT license](LICENSE).

## Example usage
gsswrap can be used to write server and client code to authenticate client connections using kerberos.

### Server example
```C
#include ""
```

### Client example


### Credential delegation


## Build from checked out repo
gsswrap uses standard autoconf/automake.
From the checked out gsswrap directory:
```sh
autoreconf -ivf
mkdir build
cd build
../configure
make
```
To run tests:
```sh
make check
```
