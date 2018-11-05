
PACKAGE= zmq-xdr-tests

CC= gcc
CFLAGS= -g -Wall -Wno-unused -Werror -Wformat
LFLAGS= -lzmq

all: dir.c dir.h rls rls_svc

rls: rls.o dir.o
	$(CC) $(CFLAGS) $(LFLAGS) $^ -o $@

rls_svc: rls_svc.o dir.o
	$(CC) $(CFLAGS) $(LFLAGS) $^ -o $@

dir.c: dir.x
	rm -f $@
	rpcgen -c -o $@ $<

dir.h: dir.x
	rm -f $@
	rpcgen -h -o $@ $<

clean:
	rm -f rls rls_svc dir.h dir.c  *.o core *~ 

