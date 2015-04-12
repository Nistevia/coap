/* coap_encode.c -- implementation for packet encoding.
 * Author: Luis Colorado <luis.colorado@ericsson.com>
 * Date: Fri Apr 10 09:54:34 EEST 2015
 * Disclaimer: (C) LUIS COLORADO.  All rights reserved.
 */

#include <assert.h>
#include "coap.h"

#ifndef DEBUG
#define DEBUG 0
#endif

#ifndef TRACE
#define TRACE 1
#endif

#if DEBUG
#   include "fprintbuf.h"
#   define DEB(args...) fprintf(stderr, args)
#   define PRB(args...) fprintbuf(stderr, args)
#else
#   define DEB(args...)
#   define PRB(args...)
#endif

#if TRACE
#   define D(x) __FILE__":%d:%s: " x, __LINE__, __func__
#else
#   define D(x) x
#endif

#define CHK(N) do{\
        size_t n = (N);\
        DEB(D("CHK(%d bytes);\n"), n);\
        if (bufsz < n) {\
            DEB(D("RETURN ==> COAP_INVALID_LENGTH(have=%d, req=%d\n"),\
                    bufsz, n);\
            return COAP_INVALID_LENGTH;\
        }\
    }while(0)

#define ACT(N) do{\
        size_t n = (N);\
        DEB(D("ACT(%d bytes);\n"), n);\
        buff  += n;\
        bufsz -= n;\
    }while(0)

size_t
coap_encode(
        const coap_msg *msg,
        uint8_t        *buff,
        size_t         *bsz_io)
{
    size_t bufsz = *bsz_io;

    DEB(D("BEGIN\n"));
    CHK(COAP_HDR_LEN); /* check we have at least space for header */

    msg->c_pktdat = buff;
    if (COAP_VERS_VALUE != msg->c_vers) {
        DEB(D("==> COAP_INVALID_VERSION(%d)\n"),
                msg->c_vers);
        return COAP_INVALID_VERSION;
    } /* if */


    return COAP_OK;
} /* coap_encode */
