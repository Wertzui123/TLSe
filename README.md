# TLSe
This V library is a wrapper around the great [TLSe library](https://github.com/eduardsui/tlse) providing easy Transport Layer Security.
<br>It is written similar to the [OpenSSL module](https://github.com/vlang/v/tree/master/vlib/net/openssl) of the V standard library.

## Advantages of TLSe over OpenSSL
* Smaller executables
* Easier deployment
* More permissive licenses (public domain, BSD or MIT)

## Limitations
* `libtomcrypt.c` does currently not compile with TCC
<br>You can still compile the this library with TCC by defining `TINYC_TLSE_COMPILE`, but it will throw a runtime error when trying to use any TLSe functions.

## License
Public domain, BSD, MIT. Choose one.