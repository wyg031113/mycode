/*
 * Please do not edit this file.
 * It was generated using rpcgen.
 */

#include "test.h"

bool_t
xdr_square_in (XDR *xdrs, square_in *objp)
{
	register int32_t *buf;

	 if (!xdr_long (xdrs, &objp->arg1))
		 return FALSE;
	return TRUE;
}

bool_t
xdr_square_out (XDR *xdrs, square_out *objp)
{
	register int32_t *buf;

	 if (!xdr_long (xdrs, &objp->res1))
		 return FALSE;
	return TRUE;
}
