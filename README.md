# XDR (eXternal Data Representation) 

"e**X**ternal **D**ata **R**epresentation (XDR) is a standard data serialization format, for uses such as computer network protocols. It allows data to be transferred between different kinds of computer systems. Converting from the local representation to XDR is called **encoding**. Converting from XDR to the local representation is called **decoding**. XDR is implemented as a software library of functions which is portable between different operating systems and is also independent of the transport layer." ..._from Wiki_

> XDR uses a base unit of 4 bytes, serialized in big-endian order; smaller data types still occupy four bytes each after encoding. Variable-length types such as string and opaque are padded to a total divisible by four bytes.

## XDR Stream Access

| Files | In-Memory | Record (TCP/IP) | 
|-|-|-|
|xdrstdio_create|xdrmem_create|xdrrec_create||

Must specify the below operations :
- XDR_ENCODE : serialise (encode) in-memory structures onto the wire.
- XDR_DECODE : deserialise (decode) from the wire into in-memory structures.
- XDR_FREE : free an in-memory structure. 

For example,
```
xdrstdio_create(&xdr, stdout, XDR_ENCODE) ;
```

## Example : XDR over ØMQ

Combined ONC/RPC rls & ØMQ Request-Reply Sample. 

```
$ make
gcc -g -Wall -Wno-unused -Werror -Wformat   -c -o rls.o rls.c
gcc -g -Wall -Wno-unused -Werror -Wformat -lzmq rls.o dir.o -o rls
gcc -g -Wall -Wno-unused -Werror -Wformat   -c -o rls_svc.o rls_svc.c
gcc -g -Wall -Wno-unused -Werror -Wformat -lzmq rls_svc.o dir.o -o rls_svc

$ ./rls_svc

$ ./rls .
Connecting to rls erver
.
..
rls.o
rls
rls_svc.o
Makefile
rls_svc
dir.c
dir.x
rls.c
rls_svc.c
dir.h
dir.o

```

## Related Posts

- [RRC 4506 : XDR External Data Representation Standard](https://tools.ietf.org/html/rfc4506)
- [XDR : Technical Notes](https://www.cisco.com/c/en/us/td/docs/ios/sw_upgrades/interlink/r2_0/rpc_pr/rpxdesc.html) from Cisco
- [XDR : Technical Notes](https://docs.oracle.com/cd/E23824_01/html/821-1671/xdrnts-21693.html#scrolltoc) from Oracle
- [GNU libc:XDR parser](https://archive.is/20150213112723/https://sourceware.org/git/?p=glibc.git;a=tree;f=sunrpc;hb=HEAD)
- [Custom XDR Stream Implementation](https://docs.oracle.com/cd/E23824_01/html/821-1671/xdrnts-65172.html)
