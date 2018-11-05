#include <zmq.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "dir.h"

static void write_request (char *request, int request_size,readdir_req *req);
static void read_reply (char *response, int response_size);
static void print_reply (const readdir_res *res);

int main (int argc, char *argv[])
{
    //WARNING : msgbuf should be adjustable to accomodate a big list of files from xdr:readdir_res
    int  msgsize = 1024;
    char msgbuf[msgsize]; 
    
    readdir_req req;

    if (argc != 2) {
		  fprintf(stderr, "Usage: %s directory\n",argv[0]);
		  exit(1);
	  }

    printf ("Connecting to rls erver\n");
    void *context = zmq_ctx_new ();
    void *requester = zmq_socket (context, ZMQ_REQ);
    zmq_connect (requester, "tcp://localhost:5555");

    req.directory = strdup(argv[1]);
    write_request (msgbuf, msgsize, &req);
    zmq_send (requester, msgbuf, msgsize , 0);

    zmq_recv (requester, msgbuf, msgsize , 0);
    read_reply( msgbuf, msgsize );

    zmq_close (requester);
    zmq_ctx_destroy (context);

    return 0;
}

static void
write_request (char *request, int request_size, readdir_req *req)
{
  XDR xdr;
  xdrmem_create (&xdr, request, request_size , XDR_ENCODE);
  if (!xdr_readdir_req (&xdr, req)) {
      fprintf (stderr, "write_request: could not encode\n");
      exit (1);
  }

  xdr_free ((xdrproc_t) xdr_readdir_req, (char *) req);
  xdr_destroy (&xdr);
  return;
}

static void
read_reply (char *response, int response_size)
{
  XDR xdr;
  readdir_res result = { 0 };

  xdrmem_create (&xdr, response, response_size , XDR_DECODE);
  if (!xdr_readdir_res (&xdr, &result)) {
      fprintf (stderr, "read_reply: could not decode\n");
      exit (1);
  }

  print_reply( &result );
	  
  xdr_free ((xdrproc_t) xdr_readdir_res, (char *) &result);
  xdr_destroy (&xdr);
}

static void
print_reply (const readdir_res *result)
{
	namelist nl;
  
  if( result->returnCode != 0) {
    printf("rls: directory read error\n");
  } else {
    for (nl = result->readdir_res_u.list;nl != NULL; nl = nl->next) {
		  printf("%s\n", nl->name);
	  }
  }
  return;
}