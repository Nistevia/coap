/* $Id: main.c.m4,v 1.7 2005/11/07 19:39:53 luis Exp $
 * Author: Luis Colorado <luis.colorado@ericsson.com>
 * Date: Sat Apr 18 20:21:41 EEST 2015
 *
 * Disclaimer:
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *  
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *  
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#define IN_COAP_INIT_C

/* Standard include files */
#include <sys/types.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>

/* constants */

/* types */

/* prototypes */

/* variables */
static char COAP_INIT_C_RCSId[]="\n$Id: main.c.m4,v 1.7 2005/11/07 19:39:53 luis Exp $\n";

/* functions */

coap_err
coap_ctx_init(coap_ctx *ctx)
{
    TR(D("BEGIN(addr=%s, port=%d)\n"),
            inet_ntoa(ctx->ctx_bind.in_addr.s_addr),
            ntohs(ctx->ctx_bind.in_port));
    TR(D("RETURN COAP_OK\n"));
    return COAP_OK;
}

/* $Id: main.c.m4,v 1.7 2005/11/07 19:39:53 luis Exp $ */
