/*
 * This is sample code generated by rpcgen.
 * These are only templates and you can use them
 * as a guideline for developing your own functions.
 */

#include "test.h"


void
square_prog_1(char *host)
{
	CLIENT *clnt;
	square_out  *result_1;
	square_in  squareproc_1_arg;

#ifndef	DEBUG
	clnt = clnt_create (host, SQUARE_PROG, SQUARE_VERS, "udp");
	if (clnt == NULL) {
		clnt_pcreateerror (host);
		exit (1);
	}
#endif	/* DEBUG */
	squareproc_1_arg.arg1 = 12;
	result_1 = squareproc_1(&squareproc_1_arg, clnt);
	if (result_1 == (square_out *) NULL) {
		clnt_perror (clnt, "call failed");
	}
	printf("out:%d\n", result_1->res1);
#ifndef	DEBUG
	clnt_destroy (clnt);
#endif	 /* DEBUG */
}


int
main (int argc, char *argv[])
{
	char *host;

	if (argc < 2) {
		printf ("usage: %s server_host\n", argv[0]);
		exit (1);
	}
	host = argv[1];
	square_prog_1 (host);
exit (0);
}
