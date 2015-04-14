/* coap.h -- definitions for CoAP protocol.
 * Author: Luis Colorado <luis.colorado@ericsson.com>
 * Date: Thu Apr  9 13:21:14 EEST 2015
 * Disclaimer: (C) LUIS COLORADO.  All rights reserved.
 */
#ifndef _COAPP_H
#define _COAPP_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#include "coap.h"

#ifndef DEBUG
#define DEBUG 0
#endif

#ifndef TRACE
#define TRACE 1
#endif

#if DEBUG
#   include "fprintbuf.h"
#   define DEB(args...) fprintf(stdout, args)
#   define PRB(args...) fprintbuf(stdout, args)
#else
#   define DEB(args...)
#   define PRB(args...)
#endif

#if TRACE
#   define D(x) __FILE__":%d:%s: " x, __LINE__, __func__
#   define TR(x) do{ DEB(D("%s;\n"), #x); x; } while(0);
#else
#   define D(x) x
#   define TR(x) do{ x; } while(0)
#endif

#define CHK(N) do{\
        size_t n = (N);\
        DEB(D("CHK(%lu bytes);\n"), n);\
        if (bufsz < n) {\
            DEB(D("RETURN ==> COAP_INVALID_LENGTH(have=%lu, req=%lu\n"),\
                    bufsz, n);\
            return COAP_INVALID_LENGTH;\
        }\
    }while(0)

#define ACT(N) do{\
        size_t n = (N);\
        DEB(D("ACT(%lu bytes);\n"), n);\
        buff  += n;\
        bufsz -= n;\
    }while(0)

#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */

#endif /* _COAPP_H */
