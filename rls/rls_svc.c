#include <zmq.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include <dirent.h>

#include "dir.h"


static void serve_request (char *request, int request_size );
static namelist  read_dir(char *dir);

int main (void)
{
  readdir_req  req;
	readdir_res  res; 
 
  //WARNING : msgbuf should be adjustable to accomodate a big list of files from xdr:readdir_res
  int  msgsize = 1024;
  char msgbuf[msgsize]; 

  void *context = zmq_ctx_new ();
  void *responder = zmq_socket (context, ZMQ_REP);
  int rc = zmq_bind (responder, "tcp://*:5555");
  assert (rc == 0);

  while (1) {

      zmq_recv (responder, msgbuf, msgsize, 0);

      serve_request( msgbuf, msgsize );

      zmq_send (responder, msgbuf, msgsize, 0);
     
  }
  return 0;
}

static void
serve_request (char *request, int request_size )
{
  XDR xdr;
  readdir_res  res; 

  DIR *dirp;

  namelist head;
	namelist nl;
	namelist *nlp;

  readdir_req req = { 0 };

  xdrmem_create (&xdr, request, request_size, XDR_DECODE);
  if (!xdr_readdir_req (&xdr, &req)) {
      fprintf (stderr, "read_request: could not decode\n");
      exit (1);
  }

  head = read_dir( req.directory ); 
  if ( head == NULL ) 
    res.returnCode = -1;
  else {
	  res.returnCode = 0;
	  nlp = &res.readdir_res_u.list;
    while ( head ) {
        nl = *nlp = (namenode *) malloc(sizeof(namenode));
        nl->name = strdup(head->name);
        nlp = &nl->next;
    	head = head->next;
    }
    *nlp = (namelist)NULL;
  }

  xdrmem_create (&xdr, request, request_size, XDR_ENCODE);
  if (!xdr_readdir_res (&xdr, &res)) {
      fprintf (stderr, "readdir_res: could not decode\n");
      exit(-1);
  }

  xdr_free ((xdrproc_t) xdr_readdir_req, (char *) &req);
  xdr_destroy (&xdr);

  xdr_free ((xdrproc_t) xdr_readdir_res, (char *) &res);
  xdr_destroy (&xdr);

  return;
}

static
namelist  read_dir(char *dir) {

    DIR *dirp;
	  struct dirent *d;
    namelist nl, head;
    nl = head = NULL;
    dirp = opendir(dir);
      
    printf("rls_svc: readdir(%s) directory\n", dir);
	  if (dirp == (DIR *)NULL)  return NULL;
    
    /* copy first entry */
    if( (d = readdir(dirp)) != NULL ) {
      head = nl = (namenode *) malloc(sizeof(namenode));  
	  if (nl == (namenode *) NULL) {
		    closedir(dirp);
		    return NULL;
	  }
	  nl->name = strdup(d->d_name);
	  nl->next = NULL;
    }

    // copy the remaining entry
	while (NULL != (d = readdir(dirp)) ) {
		nl->next = (namenode *) malloc(sizeof(namenode));  
		nl = nl->next; nl->next = NULL;
		if (nl == (namenode *) NULL) {
		    closedir(dirp);
		    return NULL;
		}
		nl->name = strdup(d->d_name);
	}
    
	return head;
}